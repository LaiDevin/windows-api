# GetProcAddress
libloaderapi.h

# __stdcall, 函数参数由右向左入栈，函数调用结束后，由被调用者（即当前函数自身清理堆栈，Windows 的api采用这种）

# __cdecl 函数参数由右向左入栈，函数调用结束后，由调用者（即谁调用整个函数，谁清理，c语言采用这种声明方式）