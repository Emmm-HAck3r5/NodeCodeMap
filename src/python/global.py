"""
THIS IS GLOBAL.PY
"""
import sys
import json
from ncc import parse
import process

print('GLOBAL.PY')

# INPUT INIT
_path = sys.argv[0]
root = parse(_path)
json_data = { # OUTPUT
    "links": [],
    "nodes": []
} 
black_list = [] # SIGN WHAT ARE VISITED

# DFS PARSE THE FILE-TREE
json = dfs_visit(root)

# SAVE JSON FILE
save_path = '' # save_path is not sure
with open(save_path, 'w') as file_obj:
    json.dump(json_data, file_obj)

# DEFINE DFS
def dfs_visit(vertex):
    black_list.append(vertex) # SIGN THAT WE VISITED THE VERTEX
    children = get_children(vertex.contents.lchild)
    for child in children:
        json = process.create_FFlink(json, vertex, child)
        if child not in black_list:
            dfs_visit(child)
    json = process.vertex_register(json, vertex) # regist members simultaneously
    json, members = process.create_FMlink(json, vertex)
    json = process.create_MMLink(json, members)
    return json

def get_children(lchild):
    children = [lchild]
    now_child = lchild
    next_child = now_child.contents.rchild
    now_child_name = now_child.contents.comp_info.contents.file_name.contents.value
    next_child_name = next_child.contents.comp_info.contents.file_name.contents.value
    while now_child_name is not next_child_name:
        now_child = next_child
        next_child = now_child.contents.rchild
        now_child_name = now_child.contents.comp_info.contents.file_name.contents.value
        next_child_name = next_child.contents.comp_info.contents.file_name.contents.value
        children.append(now_child)
    return children

