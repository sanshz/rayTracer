# Fast Image Ray Tracer (C++23)

Following *Ray Tracing* book series by *Peter Shirley*

- Book One 'Ray Tracing in One Weekend' done
- Book Two Chapter Two done

# Render

At 640x360 resolution and 96 samples per pixel it took ~1.5 hours (4931.58 seconds)

![Render](bin/render.png)

# Building

Tools:
- MSVC (cl.exe)
- x64 Native Tools Command Prompt
- VS Code

My `tasks.json` for build:
```bash
"args": [
  "/O2", "/EHsc", "/nologo",
  "/I${workspaceFolder}\\src\\private\\",
  "/I${workspaceFolder}\\src\\public\\",
  "/Fe${workspaceFolder}\\bin\\${fileBasenameNoExtension}.exe",
  "/std:c++20",
  "${fileDirname}\\**.cpp"
],
"options": {
  "cwd": "${workspaceFolder}\\bin\\intermediates\\"
}
```
