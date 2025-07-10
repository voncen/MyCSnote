# 简洁的函数参数日志记录方案

以下是一个简洁、可复用的参数日志记录实现方案，结合装饰器和类实现，方便在项目中快速集成：

## 1. 基础装饰器实现

```python
import logging
import functools
import inspect

def log_arguments(logger=None, level=logging.DEBUG, exclude=None):
    """
    记录函数所有输入参数的装饰器
    
    参数:
        logger: 日志记录器对象 (默认使用模块级日志)
        level: 日志级别 (默认DEBUG)
        exclude: 要排除的参数列表
    """
    if exclude is None:
        exclude = []
    
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            # 获取函数签名
            sig = inspect.signature(func)
            bound_args = sig.bind(*args, **kwargs)
            bound_args.apply_defaults()
            
            # 构建参数字典 (排除敏感参数)
            params = {}
            for name, value in bound_args.arguments.items():
                if name not in exclude:
                    params[name] = value
            
            # 获取日志记录器
            used_logger = logger or logging.getLogger(func.__module__)
            
            # 记录参数
            used_logger.log(
                level,
                f"调用 {func.__qualname__}",
                extra={
                    "function": func.__qualname__,
                    "module": func.__module__,
                    "params": params
                }
            )
            
            return func(*args, **kwargs)
        return wrapper
    return decorator
```

### 使用示例
```python
import logging

logging.basicConfig(level=logging.DEBUG, format="%(asctime)s [%(levelname)s] %(message)s")

@log_arguments(exclude=["password"])
def authenticate(username: str, password: str, timeout: int = 30):
    # 认证逻辑...
    return True

# 调用函数
authenticate("john_doe", "secret123", timeout=15)
```

### 输出日志
```
2023-10-15 14:30:45 [DEBUG] 调用 authenticate
   参数: {
        "username": "john_doe",
        "timeout": 15
    }
```

## 2. 高级类实现（支持更多功能）

```python
import logging
import functools
import inspect
import json
from typing import Callable, Dict, List, Optional

class ArgumentLogger:
    """高级参数日志记录器"""
    
    def __init__(
        self,
        logger: Optional[logging.Logger] = None,
        level: int = logging.DEBUG,
        max_length: int = 100,
        redact_keys: List[str] = None,
        log_return: bool = False
    ):
        """
        初始化参数日志记录器
        
        参数:
            logger: 日志记录器对象
            level: 日志级别
            max_length: 参数值最大长度
            redact_keys: 需要脱敏的参数名
            log_return: 是否记录返回值
        """
        self.logger = logger
        self.level = level
        self.max_length = max_length
        self.redact_keys = redact_keys or ["password", "token", "secret"]
        self.log_return = log_return
        
    def __call__(self, func: Callable):
        """作为装饰器使用"""
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            # 获取调用上下文
            module_name = func.__module__
            func_name = func.__qualname__
            
            # 获取实际使用的日志记录器
            logger = self.logger or logging.getLogger(module_name)
            
            # 记录输入参数
            params = self._get_arguments(func, args, kwargs)
            self._log_call(logger, func_name, params)
            
            try:
                # 执行函数
                result = func(*args, **kwargs)
                
                # 记录返回值
                if self.log_return:
                    self._log_return(logger, func_name, result)
                
                return result
            except Exception as e:
                # 记录异常
                self._log_exception(logger, func_name, e)
                raise
        
        return wrapper
    
    def _get_arguments(self, func: Callable, args: tuple, kwargs: dict) -> Dict:
        """获取函数参数并应用过滤"""
        sig = inspect.signature(func)
        bound_args = sig.bind(*args, **kwargs)
        bound_args.apply_defaults()
        
        params = {}
        for name, value in bound_args.arguments.items():
            # 脱敏处理
            if name in self.redact_keys:
                params[name] = "***REDACTED***"
            else:
                # 处理过长值
                str_val = str(value)
                if len(str_val) > self.max_length:
                    params[name] = f"{str_val[:self.max_length]}... [TRUNCATED]"
                else:
                    params[name] = value
        
        return params
    
    def _log_call(self, logger: logging.Logger, func_name: str, params: Dict):
        """记录函数调用"""
        param_str = json.dumps(params, indent=4, default=str)
        logger.log(
            self.level,
            f"调用 {func_name}\n参数:\n{param_str}",
            extra={"function": func_name, "params": params}
        )
    
    def _log_return(self, logger: logging.Logger, func_name: str, result):
        """记录函数返回结果"""
        result_str = str(result)
        if len(result_str) > self.max_length:
            result_str = f"{result_str[:self.max_length]}... [TRUNCATED]"
        
        logger.log(
            self.level,
            f"{func_name} 返回: {result_str}",
            extra={"function": func_name, "return_value": result}
        )
    
    def _log_exception(self, logger: logging.Logger, func_name: str, exception: Exception):
        """记录函数异常"""
        logger.error(
            f"{func_name} 执行失败: {str(exception)}",
            extra={"function": func_name, "exception": str(exception)}
        )
```

### 使用示例

```python
# 创建配置好的日志记录器实例
arg_logger = ArgumentLogger(
    level=logging.INFO,
    max_length=50,
    redact_keys=["api_key", "password"],
    log_return=True
)

# 应用于函数
@arg_logger
def process_data(
    data_source: str, 
    api_key: str, 
    options: dict,
    timeout: int = 30
):
    # 数据处理逻辑...
    return {"status": "success", "items_processed": 1500}

# 调用函数
process_data(
    "https://api.example.com/data",
    "sk-1234567890abcdef",
    {"filter": "active", "max_results": 100},
    timeout=20
)
```

### 输出日志
```
2023-10-15 14:45:22 [INFO] 调用 process_data
参数:
{
    "data_source": "https://api.example.com/data",
    "api_key": "***REDACTED***",
    "options": {
        "filter": "active",
        "max_results": 100
    },
    "timeout": 20
}

2023-10-15 14:45:23 [INFO] process_data 返回: {'status': 'success', 'items_processed': 1500}
```

## 3. 类方法装饰器

对于类中的方法，我们需要额外处理 `self` 或 `cls` 参数：

```python
class MethodArgumentLogger(ArgumentLogger):
    """专用于类方法的参数日志记录器"""
    
    def _get_arguments(self, func: Callable, args: tuple, kwargs: dict) -> Dict:
        """获取方法参数，跳过self/cls"""
        # 获取绑定签名
        sig = inspect.signature(func)
        bound_args = sig.bind(*args, **kwargs)
        bound_args.apply_defaults()
        
        # 跳过第一个参数 (self/cls)
        params = {}
        param_names = list(bound_args.arguments.keys())[1:]  # 跳过第一个
        
        for name in param_names:
            value = bound_args.arguments[name]
            
            # 脱敏处理
            if name in self.redact_keys:
                params[name] = "***REDACTED***"
            else:
                # 处理过长值
                str_val = str(value)
                if len(str_val) > self.max_length:
                    params[name] = f"{str_val[:self.max_length]}... [TRUNCATED]"
                else:
                    params[name] = value
        
        return params
```

### 在类中使用示例

```python
class DataProcessor:
    # 创建方法专用的日志记录器
    method_logger = MethodArgumentLogger(
        redact_keys=["password"],
        log_return=True
    )
    
    @method_logger
    def transform_data(self, dataset: list, options: dict, verbose: bool = False):
        """处理数据集"""
        # 转换逻辑...
        return {"status": "completed", "count": len(dataset)}
    
    @classmethod
    @method_logger
    def create_processor(cls, config: dict, api_key: str):
        """创建处理器"""
        # 创建逻辑...
        return cls(config, api_key)

# 使用
processor = DataProcessor()
processor.transform_data(
    [1, 2, 3, 4, 5],
    {"method": "normalize"},
    verbose=True
)

DataProcessor.create_processor(
    {"cache_size": 100},
    "sk-1234567890"
)
```

## 4. 高级特性：上下文管理器

对于需要记录参数但不适合装饰器的场景，可以使用上下文管理器：

```python
class LogArgumentsContext:
    """参数日志记录上下文管理器"""
    
    def __init__(
        self,
        logger: logging.Logger,
        func_name: str,
        level: int = logging.DEBUG,
        max_length: int = 100,
        redact_keys: List[str] = None
    ):
        self.logger = logger
        self.func_name = func_name
        self.level = level
        self.max_length = max_length
        self.redact_keys = redact_keys or []
        self.params = {}
    
    def __enter__(self):
        return self
    
    def __exit__(self, exc_type, exc_val, exc_tb):
        # 记录参数
        param_str = json.dumps(self.params, indent=4, default=str)
        self.logger.log(
            self.level,
            f"{self.func_name} 调用参数:\n{param_str}",
            extra={"function": self.func_name, "params": self.params}
        )
    
    def add_param(self, name: str, value):
        """添加参数并应用格式化"""
        if name in self.redact_keys:
            self.params[name] = "***REDACTED***"
        else:
            str_val = str(value)
            if len(str_val) > self.max_length:
                self.params[name] = f"{str_val[:self.max_length]}... [TRUNCATED]"
            else:
                self.params[name] = value
```

### 使用示例

```python
def complex_operation(data_source, filters, api_key):
    # 创建日志上下文
    with LogArgumentsContext(
        logging.getLogger(__name__),
        "complex_operation",
        redact_keys=["api_key"]
    ) as log_ctx:
        
        log_ctx.add_param("data_source", data_source)
        log_ctx.add_param("filters", filters)
        log_ctx.add_param("api_key", api_key)
        
        # 实际业务逻辑...
        result = perform_operation(data_source, filters, api_key)
        
        return result
```

## 最佳实践建议

1. **敏感数据处理**：
   - 始终过滤密码、API密钥等敏感信息
   - 使用 `redact_keys` 参数指定需要脱敏的字段

2. **性能优化**：
   - 对于高频调用的函数，设置 `level=logging.DEBUG` 避免生产环境性能影响
   - 使用 `max_length` 限制长参数值的记录

3. **结构化日志**：
   - 在 `extra` 参数中提供结构化数据
   - 与 ELK 或 Splunk 等日志系统集成时，便于搜索和分析

4. **异常处理**：
   - 装饰器自动捕获并记录异常
   - 保持原始异常堆栈信息

5. **部署策略**：
   ```mermaid
   graph LR
       A[开发环境] --> B[启用所有参数日志]
       C[测试环境] --> D[启用关键参数日志]
       E[生产环境] --> F[仅记录元数据]
   ```

通过这些实现，您可以：
- 简洁地在日志中记录函数参数
- 灵活控制日志详细程度
- 保护敏感信息
- 在函数和方法中复用
- 集成到现有日志系统中
- 根据环境调整日志级别

这种方案特别适用于调试复杂函数调用、审计关键操作以及分析生产环境中的问题场景。