import bpy
import os

# Get active object
obj = bpy.context.active_object
mesh = obj.data
mesh.calc_loop_triangles()

world_matrix = obj.matrix_world

# Ensure the object has vertex colors
if not mesh.vertex_colors:
    print("No vertex color layers found on the object.")
    exit()

# Access the first vertex color layer (if multiple layers exist, you can adjust this)
vertex_colors = mesh.vertex_colors.active.data

# Choose output file path — this saves it next to your .blend file
output_path = bpy.path.abspath("//vertex_color_output.txt")

with open(output_path, "w") as f:
    for tri in mesh.loop_triangles:
        for loop_index in tri.loops:
            # Get the vertex index for the current loop
            vertex_index = mesh.loops[loop_index].vertex_index
            # Get the color for the current loop (vertex color)
            color = vertex_colors[loop_index].color  # color is a bpy_prop_array
            # Access the color components using indices (0: Red, 1: Green, 2: Blue, 3: Alpha)
            f.write(f"{color[0]} {color[1]} {color[2]} {color[3]}\n")

print(f"Vertex color data written to: {output_path}")

import bpy
import os

# Get active object
obj = bpy.context.active_object
mesh = obj.data
mesh.calc_loop_triangles()

world_matrix = obj.matrix_world

# Choose output file path — this saves it next to your .blend file
output_path = bpy.path.abspath("//vertex_output.txt")

with open(output_path, "w") as f:
    for tri in mesh.loop_triangles:
        for loop_index in tri.loops:
            vertex_index = mesh.loops[loop_index].vertex_index
            vertex = mesh.vertices[vertex_index]
            world_coord = world_matrix @ vertex.co
            f.write(f"{world_coord.x} {world_coord.y} {world_coord.z}\n")

print(f"Vertex data written to: {output_path}")

import bpy
import os

# Get active object
obj = bpy.context.active_object
mesh = obj.data
mesh.calc_loop_triangles()

world_matrix = obj.matrix_world
normal_matrix = world_matrix.to_3x3().inverted().transposed()  # For transforming normals

# Choose output file path — this saves it next to your .blend file
output_path = bpy.path.abspath("//normal_output.txt")

with open(output_path, "w") as f:
    for tri in mesh.loop_triangles:
        for loop_index in tri.loops:
            vertex_index = mesh.loops[loop_index].vertex_index
            normal = mesh.vertices[vertex_index].normal
            world_normal = normal_matrix @ normal
            world_normal.normalize()
            f.write(f"{world_normal.x} {world_normal.y} {world_normal.z}\n")

print(f"Normal data written to: {output_path}")

