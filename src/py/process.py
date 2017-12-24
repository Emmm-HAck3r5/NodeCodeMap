"""
PROCESS JSON FILE 
"""

# LINK PROCESS
def create_FFlink(json, source, target): # File-to-File link
    source_comp = source.contents.comp_info.contents
    target_comp = target.contents.comp_info.contents
    source_file_name = getchararr(source_comp.file_path).split('\\')[-1]
    #print("SFN"+getchararr(source_comp.file_path).split('\\')[-1])
    target_file_name = getchararr(target_comp.file_path).split('\\')[-1]
    link = {
        "source": source_file_name, # file_name's c_type unknown
        "target": target_file_name,
        "type": "FF"
    }
    json['links'].append(link)
    return json

def create_FMlink(json, vertex): # File-to-Member link
    vertex_comp = vertex.contents.comp_info.contents 
    vertex_file_name = getchararr(vertex_comp.file_path).split('\\')[-1]
    members = get_members(vertex)
    cFunc, cVar, cType = members
    func_link = {
        "source": vertex_file_name, # file_name's c_type unknown
        "target": getenstringvalue(cFunc.func_name), # name's c_type unknown
        "type": "FM"
    }
    type_link = {
        "source": vertex_file_name, # file_name's c_type unknown
        "target": getenstringvalue(cVar.var_name), # name's c_type unknown
        "type": "FM"
    }
    var_link = {
        "source": vertex_file_name, # file_name's c_type unknown
        "target": getenstringvalue(cType.type_name), # name's c_type unknown
        "type": "FM"
    }
    json['links'].extend([func_link, type_link, var_link])
    return json, members

def create_MMLink(json, members):
    # cFunc, _, cType = members
    # json = get_func_var_links(json, cFunc)
    # json = get_type_var_links(json, cType)
    # json = get_func_func_links(json, cFunc)
    return json

# # LINKS COMMON PROCESS
# def get_func_var_links(json, cFunc):
#     now_params = cFunc.parameter.contents
#     next_params = now_params.next.contents
#     now_params_name = now_params.var_name.contents.value
#     next_params_name = next_params.var_name.contents.value
#     while now_params_name is not next_params_name:
#         link = {
#             "source": getenstringvalue(cFunc.func_name),
#             "target": now_params_name,
#             "type": "MM"
#         }
#         json["links"].append(link)
#         now_params = next_params
#         next_params = now_params.next.contents
#         now_params_name = now_params.var_name.contents.value
#         next_params_name = next_params.var_name.contents.value
#     return json

# def get_type_var_links(json, cType):
#     now_member = cType.parameter.contents
#     next_member = now_member.next.contents
#     now_member_name = now_member.name.contents.value
#     next_member_name = next_member.name.contents.value
#     while now_member_name is not next_member_name:
#         link = {
#             "source": cType.name.contents.value,
#             "target": now_member_name,
#             "type": "MM"
#         }
#         json["links"].append(link)
#         now_member = next_member.parameter.contents
#         next_member = now_member.next.contents
#         now_member_name = now_member.name.contents.value
#         next_member_name = next_member.name.contents.value
#     return json

# def get_func_func_links(json, cFunc):
#     func_body = cFunc.func_body.contents.values
#     func_name = cFunc.name.contents.value
#     dataNode = json["nodes"]
#     for node in dataNode:
#         nodeName = node["id"]
#         if(node["type"] is not "func" or nodeName is func_name):
#             continue
#         node_posi = func_body.find(nodeName)
#         if node_posi == -1: # not found
#             continue
#         link = {
#             "source": func_name,
#             "target": nodeName,
#             "type": "MM"
#         }
#         json["links"].append(link)
#     return json

# NODE PROCESS
def vertex_register(json, vertex):
    vertex_comp = vertex.contents.comp_info.contents
    # print(getchararr(vertex_comp.file_path).split('\\')[-1])
    try:
        node = {
            "id": getchararr(vertex_comp.file_path).split('\\')[-1],  # file_name's c_type unknown
            "filecontent": getenstringvalue(vertex.contents.pfile.contents.file), # decl's c_type unknown
            "type": "file"
        }
    except:
        node = {
            "id": getchararr(vertex_comp.file_path).split('\\')[-1],  # file_name's c_type unknown
            "filecontent": " ", # decl's c_type unknown
            "type": "file"
        }

    json["nodes"].append(node)
    
    members = get_members(vertex)
    json = members_register(json, members)

    return json

def members_register(json, members):
    cFunc, cVar, cType = members

    func_node = func_application(cFunc)
    var_node = var_application(cVar)
    # macro_node = macro_application(cMacro)
    type_node = type_application(cType)

    json["nodes"].extend([func_node, var_node, type_node])
    return json

def func_application(cFunc):
    func_nodes = []
    # print(getenstringvalue(cFunc.func_name))
    funcs = getallfunc(cFunc)
    # print(funcs)
    for func in funcs:
        global func_node 
        func_node = {
            "id": getenstringvalue(func.func_name), # func_name's c_type unknown
            "class": "func",
            "codeContent": getenstringvalue(func.comp_info.contents.decl) # func_name's c_type unknown
        }
        # print("================HERE===============")
        func_nodes.append(func_node)
    return func_nodes

def var_application(cVar):
    var_nodes = []
    # print(getenstringvalue(cVar.var_name))
    vars = getallvar(cVar)
    for var in vars:
        global var_node
        var_node = {
            "id": getenstringvalue(var.var_name), # var_name's c_type unknown
            "class": "var"
        }
        # print(var_node)
        var_nodes.append(var_node)
    # print(var_nodes)
    return var_nodes

# def macro_application(cMacro):
#     macro_node = {
#         "id": cMacro.name.contents.value, # macro_name's c_type unknown
#         "class": "macro"
#     }
#     return macro_node

def type_application(cType):
    type_nodes = []
    # print(getenstringvalue(cType.type_name))
    types = getalltype(cType)
    for typ in types:
        type_node = {
            "id": getenstringvalue(cType.type_name), # type_name's c_type unknown
            "class": "type"
        }
        # print(type_node)
        type_nodes.append(type_node)
    # print(type_nodes)
    return type_nodes

# COMMON PROCESS
def get_members(vertex):
    vertex_cFile = vertex.contents

    cFunc = vertex_cFile.function_list.contents
    cVar = vertex_cFile.var_list.contents
    # cMacro = vertex_cFile.macro_list.contents
    cType = vertex_cFile.type_list.contents

    return [cFunc, cVar, cType]

def getchararr(char_arr):
    result = ''
    for char in char_arr:
        asciichar = ord(char.decode())
        if asciichar == 0:
            break
        result += char.decode()
    return result
def getvertexname(vertex):
    return getchararr(vertex.contents.comp_info.contents.file_path).split('\\')[-1]

def getenstringvalue(ehstring_ptr):
    ehstring_value = ehstring_ptr.contents.value
    result = ''
    for asc in ehstring_value:
        if asc == 0:
            break
        try:
            char = chr(asc)
        except:
            char=' '
            # print(char)
            # print(asc)
        result += char
    # print(result)
    return result

def getallfunc(cFunc):
    funcs = []
    next_func = cFunc.next.contents
    next_func_name = getenstringvalue(next_func.func_name)
    # print(next_func_name)
    while next_func_name != '':
        funcs.append(next_func)
        next_func = next_func.next.contents
        next_func_name = getenstringvalue(next_func.func_name)
    return funcs

def getallvar(cVar):
    vars = []
    next_var = cVar.next.contents
    next_var_name = getenstringvalue(next_var.var_name)
    # print(next_var_name)
    while next_var_name != '':
        vars.append(next_var)
        next_var = next_var.next.contents
        next_var_name = getenstringvalue(next_var.var_name)
    return vars

def getalltype(cType):
    types = []
    next_type = cType.next.contents
    next_type_name = getenstringvalue(next_type.type_name)
    # print(next_type_name)
    while next_type_name != '':
        types.append(next_type)
        next_type = next_type.next.contents
        next_type_name = getenstringvalue(next_type.type_name)
    return types