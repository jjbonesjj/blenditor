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
import bpy;
import sys;
import os 
dirPath = os.path.dirname(os.path.realpath(__file__))
print(dirPath)
sys.path.append(dirPath + "\\intern\\");
import convexify;
import pprint;
from ctypes import *;

class ExportLevel(bpy.types.Operator):
    """Export Level"""
    bl_idname = "export_level.cyl"
    bl_label = "Exports CYL"
    bl_options = {'PRESET'} 
    filename_ext = ".cyl"

    def execute(self, context): # called by blender
        print("exporting")
        obj = bpy.context.active_object
        mesh = obj.data

        Point = c_float * 3;
        vertexArray = (Point * len(mesh.vertices))()
        for i in range(mesh.vertices):
            coord = mesh.vertices[i]
            point = Point(coord.x, coord.y, coord.z)
            vertexArray[i] = point;

        print("calling convexifyMesh")
        pprint.pprint(mesh)
        print(mesh)
        print(mesh.vertices[0])

        print(convexify.convexifyMesh(vertexArray))
        print("finished calling convexifyMesh")

        return {'FINISHED'} # tell blender success

#def menu_func_import(self, context):
#    self.layout.operator(ImportLevel.bl_idname, text="Cy Level (.cyl)")


def menu_func_export(self, context):
    self.layout.operator(ExportLevel.bl_idname, text="Cy Level (.cyl)")

# wtb better synonym for class.
breeds = (
    ExportLevel,    
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
    print("1")
    cdll.LoadLibrary("testing.dll");
    print("2")
    conv = CDLL("testing.dll");
    print("3")
    print(conv)
    print("\n\n\n")
    val = conv.convexifyStuff();
    print("4")
    print(val)

    ExportLevel.execute(None, None)