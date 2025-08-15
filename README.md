# Fast Image Ray Tracer (C++23)

Following 'Ray Tracing in One Weekend' guide by Peter Shirley

Completed till chapter 8

# Building
Tools:
- MSVC++ (cl.exe)
- x64 Native Tools Command Prompt
- VS Code

My `tasks.json` for build:
```bash
"args": [
  "/Zi", "/EHsc", "/nologo",
  "/I${workspaceFolder}\\src\\private\\",
  "/I${workspaceFolder}\\src\\public\\",
  "/Fe${workspaceFolder}\\bin\\${fileBasenameNoExtension}.exe",
  "/permissive", "/Wall", "/WX",
  "/std:c++20",
  "${fileDirname}\\**.cpp"
],
"options": {
  "cwd": "${workspaceFolder}\\bin\\intermediates\\"
}
```

# Render

![Render](bin/render.png)
