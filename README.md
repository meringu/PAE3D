# PAE3D

Victoria University COMP308 project 2014

## compile

	make

## run default cube

	./build/PAE3D

## to open obj

	./build/PAE3D -o models/Teapot.obj

## to operate

Top toolbar buttons have short cut keys written on them; They work as follows.

### Camera

- `middle click` + `drag` to rotate.
- `ctrl` + `middle click` + `drag` to zoom.
- `shift` + `middle click` + `drag` to pan.

### Selection

- `f`, `e`, `v` toggle selection mode to faces, edges and verticies respectivly.
- `a` to select all.
- right click to select face, edge or vertex.

### Move/Scale

- `m` to toggle to move mode
- `s` to toggle to scale mode
- `left click` + `drag` handles to move/scale

### Regional Extrude

- `x` to extrude selected faces, note you need to move or scale after as it extrudes in place.

### Merge

- `l` to merge all selected verticies, edges, faces to the center of the selection (handle position)

### Subdivide

- `d` to subdivide selected faces, if none are selected will subdivide the whole model

### Smooth

- `g` to smooth selected faces, if none are selected will smooth the whole model

### Colour

- `c` to enable/disable coulouring by left click. colouring a face will set it to the material selected in the colour window
- in the bottom left of the screen.

- `+` button to add new material.
- `<`, `>` buttons to navigate the materials.
- sliders to change material parameters.

### Renderer

- `p` to switch to render mode, only faces are drawn with gourad shading. A skybox is added for aesthetics.

### Copy

- `ctrl` + `c` to make a copy of the current selection, note use move/scale after as copy is in place.

### Delete

- `del` to delete all selected faces, edges and verticies and any edges and faces attached to selected verticies

## Vagrant

A Vagrantfile is provided for testing on Linux.
