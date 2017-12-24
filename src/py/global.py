"""
THIS IS GLOBAL.PY
"""
import sys
import os
import json
from ncc import parse
import process
import ctypes as ct
#from path_cfg import glb_root_path as glb_root_path


# print('GLOBAL.PY')


# INPUT INIT
# _path = sys.argv[0]
# glb_root_path="D:\\Emmm_HAck3r5\\NCM_release\\"
glb_root_path=sys.path[0]
tgt_dir=sys.argv[1]
# print(tgt_dir)
_path = bytes(tgt_dir, encoding="ascii")
# _path = bytes("D:\\Emmm_HAck3r5\\NCM_project\\test", encoding="ascii")
root = parse(_path)
json_data = { # OUTPUT
    "links": [],
    "nodes": []
} 
black_list = [] # SIGN WHAT ARE VISITED

# DEFINE DFS
def dfs_visit(vertex):
    global json_data
    global black_list
    vertex_name = process.getvertexname(vertex)
    black_list.append(vertex_name) # SIGN THAT WE VISITED THE VERTEX
    children = get_children(vertex.contents.lchild)
    for child in children:
        json_data = process.create_FFlink(json_data, vertex, child)
        child_name = process.getvertexname(child)
        # print(child_name, black_list)
        # print(child_name in black_list)
        if child_name not in black_list:
            dfs_visit(child)
    json_data = process.vertex_register(json_data, vertex) # regist members simultaneously
    # print(json_data)
    json_data, members = process.create_FMlink(json_data, vertex)
    json_data = process.create_MMLink(json_data, members)
    return json_data

def get_children(lchild):
    children = [lchild]
    now_child = lchild
    next_child = now_child.contents.rchild
    string2 =  now_child.contents.comp_info.contents.file_path
    now_child_path = now_child.contents.comp_info.contents.file_path
    next_child_path = next_child.contents.comp_info.contents.file_path
    now_child_name = process.getchararr(now_child_path).split('\\')[-1]
    next_child_name = process.getchararr(next_child_path).split('\\')[-1]
    # print(now_child_name, next_child_name)
    while now_child_name != next_child_name:
        # print("_"+now_child_name+"_")
        # print("_"+next_child_name+"_")
        now_child = next_child
        next_child = now_child.contents.rchild
        now_child_path = now_child.contents.comp_info.contents.file_path
        next_child_path = next_child.contents.comp_info.contents.file_path
        now_child_name = process.getchararr(now_child_path).split('\\')[-1]
        next_child_name = process.getchararr(next_child_path).split('\\')[-1]
        children.append(now_child)
    return children


# DFS PARSE THE FILE-TREE
json_data = dfs_visit(root)
# print(json_data)
# SAVE JSON FILE
save_path = glb_root_path+'\\..\\dat.json' # save_path is not sure
print(save_path)
# os.mknod(save_path)
with open(save_path, 'w') as file_obj:
    json.dump(json_data, file_obj)