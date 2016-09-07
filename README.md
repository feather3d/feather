Feather
=======

![Feather Logo](https://raw.githubusercontent.com/richardlayman/feather/master/assets/images/logo.png)

3d modeling and animation application.

![Feather UI](https://raw.githubusercontent.com/richardlayman/feather/master/assets/images/ui_v0_1.png)

Current Status
---------------
Feather is still a WIP that has not yet reached v0.1. I hope to release v0.1 sometime in 2016. Below is a list of tasks to be completed for the v0.1 release.
* Finish up Keyframe Editor
* Add DopeSheet Editor
* Undo/Redo system
* Finish up nodes

Introduction
---------------
Feather is a 3D application targeted for small studios and indie animators. Feather focuses on being easy to modify with idea being that it's a tool that the artist can change to suite their needs. This could be modifying the UI, creating new nodes or commands, or using Feather from the command line. This is achieved by having a fully scripted QML interface and a very simple way of creating plugins. Everything in Feather is a node who's output fields can be plugged into any input node field of the same type. This provides the user with a great amount of flexiblity when dealing with complex problems.

v0.1 Features
---------------
* C++ core
* QtQuick interface
* Open/Save feather project files
* DAG scenegraph
* C++ Plugin architecture
* Import/Export obj files
* Outliner Editor
* SceneGraph Editor
* Field Editor
* Layer Editor
* Keyframe Editor
* DopeSheet Editor
* 3D Viewport Editor
* Objects - Curves, Plane, Cube, MeshShape
* Modify Tools - Bevel, Extrude, Inset
* Selection Tools - Pick, Box, Ring, Loop
* Component Selection - Vertex, Edge, Face, Object
* Transformation Tools (all components) - Translate, Rotate, Scale

