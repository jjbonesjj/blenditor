#    Copyright (C) 2018 Scott Checko. All Rights Reserved.

#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.

#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.

#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.

bl_info = {
    "name": "Blenditor",
    "description": "Adds level editor and level exporter/importer functionality.",
    "author": "Scott Checko",
    "version": (0, 0, 1),
    "blender": (2, 79, 0),
    "location": "File > Import-Export",
    "warning": "", # used for warning icon and text in addons panel
    "support": "TESTING",
    "category": "Development"
}
print("\n\nPYTHON START\n");

import os;
import sys;
dirPath = os.path.dirname(os.path.realpath(__file__))
print(dirPath)
sys.path.append(dirPath + "\\intern\\");
sys.path.append(dirPath);

import time;
import bpy;

from ctypes import *;

# need to both import AND use ctypes.CDLL? dafuq?
import convexify;
#convexify = CDLL("convexify.pyd");

from blenditor import *;

class Point(Structure):
    _fields_ = [
        ("x", c_float),
        ("y", c_float),
        ("z", c_float)
    ]

class ExportLevel(bpy.types.Operator):
    """Export Level"""
    bl_idname = "export_level.cyl"
    bl_label = "Exports CYL"
    bl_options = {'PRESET'} 
    filename_ext = ".cyl"

    def execute(self, context): # called by blender
        print("exporting\n\n")


        for ob in bpy.data.objects:
            print (ob.name)

        #obj = bpy.context.active_object
        #mesh = obj.data

        #vertexArray = (Point * len(mesh.vertices))()
        #for index, vert in enumerate(mesh.vertices):
        #    coord = vert.co
        #    point = Point(coord.x, coord.y, coord.z)
        #    vertexArray[index] = point;*/

        #print(dir(bpy.data.meshes['Cube'].polygons[0].vertices))
        #print(bpy.data.meshes['Cube'].polygons[0].vertices[1])

        #convexify.convexifyMesh.restype = c_bool
        result = convexify.convexifyMesh(bpy.data, bpy.context)


        #print("result is ", result)
        print("finished calling convexifyMesh")

        return {'FINISHED'} # tell blender success

#def menu_func_import(self, context):
#    self.layout.operator(ImportLevel.bl_idname, text="Cy Level (.cyl)")


def menu_func_export(self, context):
    self.layout.operator(ExportLevel.bl_idname, text="Cy Level (.cyl)")

# wtb better synonym for class.
breeds = (
    ExportLevel,
   # Blenditor    
) 

def register():
    print("hello")
    for breed in breeds:
        bpy.utils.register_class(breed)
    
    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    print("goodbye")
    for breed in breeds:
        bpy.utils.unregister_class(breed)

    bpy.types.INFO_MT_file_export.remove(menu_func_export)

# This allows you to run the script directly from blenders text editor
# to test the addon without having to install it.
if __name__ == "__main__":
    register()
if __name__ == "__main__" and bpy.app.background:
    ExportLevel.execute(None, None)