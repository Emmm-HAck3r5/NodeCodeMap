"""
THIS IS GLOBAL.PY
"""
import sys
import json
from ncc import parse
_path = sys.argv[0]
root = parse(_path)

import process

print('GLOBAL.PY')

json_data = { # OUTPUT
    "links": [],
    "nodes": []
} 
black_list = [] # SIGN WHAT ARE VISITED

json = dfs_visit(root)

save_path = '' # save_path is not sure
with open(save_path, 'w') as file_obj:
    json.dump(json_data, file_obj)


def dfs_visit(vertex):
    black_list.append(vertex) # SIGN THAT WE VISITED THE VERTEX
    children = get_children(vertex.contents.lchild)
    for child in children:
        json = process.create_FFlink(json, vertex, child)
        if child not in black_list:
            dfs_visit(child)
    json = process.vertex_register(json, vertex) # regist members simultaneously
    json = process.create_FMlink(json, vertex) # create M-M links simultaneously
    return json

def get_children(lchild):
    children = [lchild]
    next_child = lchild
    while next_child is not next_child.contents.rchild:
        next_child = next_child.contents.rchild
        children.append(next_child)
    return children

