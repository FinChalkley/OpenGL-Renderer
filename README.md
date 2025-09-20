This is my university openGL renderer project but made public. Sorry no commit history available. Please use Cmake version 3.29.7 to download, other later bersion have issues. Link to file download location - "https://cmake.org/files/v3.29".

Renderer for use across multiple modules in DMU.

Repo includes DemonRender and an application which uses this renderer.

DemonRenderer is implemented as a separate, static library. It abstracts away the OpenGL implementation details and provides tools for building and managing the rendering pipeline, input handling, asset management, etc.

There is an application ‘application’ which uses this renderer. The application has classes and scripts.This is where we will be working in this module. We will be using the DemonRenderer library to build our application.

Use this repository to build your coursework. The application uses Layers. We'd suggest to use a Layer for each lab, and possibly instead of branches.

Commit regularly.
