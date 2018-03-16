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

import bpy;

class Blenditor(bpy.types.Panel):
    """Blenditor Main"""
    bl_label="Blenditor";
    bl_space_type="PROPERTIES";
    bl_region_type="WINDOW";


    def __init__(self):
        print("init blenditor");

    def draw(self, context):
        print("drawing");
        
        layout = self.layout
 
        layout.label("First row")
        row = self.layout.row();
        row.label(text="hello");
        self.layout.operator("hello.hello", text='Ciao').country = "Italy"

class OBJECT_OT_HelloButton(bpy.types.Operator):
    bl_idname = "hello.hello"
    bl_label = "Say Hello"
    country = bpy.props.StringProperty()
 
    def execute(self, context):
        
        # add visual bounds
        WIDTH = 1.0
        HEIGHT = 1.0
        DEPTH = 3.0
        bpy.ops.mesh.primitive_cube_add();
        obj = bpy.context.object
        bpy.ops.transform.resize(value=(WIDTH, HEIGHT, DEPTH))
        obj.hide_select = True
        obj.data.update()

        if self.country == '':
            print("Hello world!")
        else:
            print("Hello world from %s!" % self.country)
        return{'FINISHED'} 


bpy.utils.register_module(__name__)



     

