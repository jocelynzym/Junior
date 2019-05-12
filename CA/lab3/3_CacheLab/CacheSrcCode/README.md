## 文件说明

-  `main_mem.sv`  和  `mem.sv`

  -主存代码

  -cache代码会调用它们

-  `cache.sv`

  -直接相连cache的代码

- `generate_cache_tb.py `

  -用于生成 cache 测试testbench的python代码

- `cache_tb.sv `

  -一个由 generate_cache_tb.py 生成的testbench



