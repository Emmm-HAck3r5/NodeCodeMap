"""
THIS IS GLOBAL.PY
"""
# import sys
# import json
# from parse import parse
# _path = sys.argv[0]
# root_p = parse(_path)

import process

print('GLOBAL.PY')

json_data = {
    "links": [],
    "nodes": []
} # OUTPUT
black_list = [] # SIGN WHAT ARE VISITED






# save_path = ''
# with open(save_path, 'w') as file_obj:
#     json.dump(json_data, file_obj)


def dfs_visit(vertex):
    black_list.append(vertex) # SIGN THAT WE VISITED THE VERTEX
    children = get_children(vertex.contents.lchild)
    for child in children:
        
        if child not in black_list:
            dfs_visit(child)

def get_children(lchild):
    children = [lchild]
    next_child = lchild.contents.rchild
    while next_child is not next_child.contents.rchild:
        children.append(next_child)
    return children

