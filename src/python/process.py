"""
PROCESS JSON FILE 
"""

# LINK PROCESS
def create_FFlink(json, source, target): # File-to-File link
    source_comp = source.contents.comp_info.contents
    target_comp = target.contents.comp_info.contents
    link = {
        "source": source_comp.file_name.contents.value, # file_name's c_type unknown
        "target": target_comp.file_name.contents.value,
        "type": "FF"
    }
    json['links'].append(link)
    return json

def create_FMlink(json, vertex): # File-to-Member link
    vertex_comp = vertex.contents.comp_info.contents 
    members = get_members(vertex)
    for mem in members:
        link = {
            "source": vertex_comp.file_name.contents.value, # file_name's c_type unknown
            "target": mem.name.contents.value, # name's c_type unknown
            "type": "FM"
        }
        json['links'].append(link)
    json = create_MMLink(json, members)
    return json

def create_MMLink(json, members): # Member-to-Member link
    for mem in members:
        for refer in mem.refer_list: # refer_list is not consistent
            link = {
                "source": mem.name.contents.value, # name's c_type unknown
                "target": refer.contents.name.contents.value,
                "type": "MM"
            }
            json['links'].append(link)
    return json

# NODE PROCESS
def vertex_register(json, vertex):
    vertex_comp = vertex.contents.comp_info.contents
    node = {
        "id": vertex_comp.file_name.contents.value,  # file_name's c_type unknown
        "decl": vertex_comp.decl.value, # decl's c_type unknown
        "fileData": vertex_comp.file_data.value,
        "lineno": vertex_comp.lineno.value
    }
    json["nodes"].append(node)
    
    members = get_members(vertex)
    json = members_register(json, members)

    return json

def members_register(json, members):
    cFunc, cVar, cMacro, cType = members

    func_node = func_application(cFunc)
    var_node = var_application(cFunc)
    macro_node = macro_application(cFunc)
    type_node = type_application(cFunc)

    json["nodes"].extend([func_node, var_node, macro_node, type_node])
    return json

def func_application(cFunc):
    func_node = {
        "id": cFunc.name.contents.value, # func_name's c_type unknown
        "class": "func"
    }
    return func_node

def var_application(cVar):
    var_node = {
        "id": cVar.name.contents.value, # var_name's c_type unknown
        "class": "var"
    }
    return var_node

def macro_application(cMacro):
    macro_node = {
        "id": cMacro.name.contents.value, # macro_name's c_type unknown
        "class": "macro"
    }
    return macro_node

def type_application(cType):
    type_node = {
        "id": cType.name.contents.value, # type_name's c_type unknown
        "class": "type"
    }
    return type_node

# COMMON PROCESS
def get_members(vertex):
    vertex_cFile = vertex.contents

    cFunc = vertex_cFile.function_list.contents
    cVar = vertex_cFile.var_list.contents
    cMacro = vertex_cFile.macro_list.contents
    cType = vertex_cFile.type_list.contents

    return [cFunc, cVar, cMacro, cType]