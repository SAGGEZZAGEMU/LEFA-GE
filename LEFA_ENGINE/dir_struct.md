SRC/
│
├── 3rdparty/           # Dependências externas (bibliotecas de terceiros, como GLM, stb_image, etc.)
│   └── <library_name>/
│
├── contents/           # Conteúdo específico do motor (texturas, shaders, scripts)
│   ├── textures/
│   ├── shaders/
│   └── scripts/
│
├── platforms/          # Código específico de plataformas (Windows, Linux, Mac)
│   ├── windows/
│   ├── linux/
│   └── macos/
│
├── render/             # Módulo de renderização (OpenGL, Vulkan, etc.)
│   ├── opengl/
│   ├── vulkan/
│   └── renderer.h
│
├── core/               # Funções principais do motor (gerenciamento de janela, entrada, etc.)
│   ├── window_manager.c
│   ├── input_manager.c
│   └── engine_core.h
│
├── utils/              # Utilitários e funções de apoio (math, logging, etc.)
│   ├── math/
│   ├── logging.c
│   └── utils.h
│
└── loadEngine.h        # Arquivo principal de entrada que inclui os módulos e inicializa o motor