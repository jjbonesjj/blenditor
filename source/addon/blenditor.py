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
        row = self.layout.row();
        row.label(text="hello");


     

