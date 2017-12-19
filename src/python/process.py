"""
PROCESS JSON FILE
"""

def create_link(json, source, target):
    source_comp = source.contents.comp_info
    target_comp = target.contents.comp_info
    link = {
        "source": source_comp.contents.file_name.contents.value,
        "target": target_comp.contents.file_name.contents.value
    }
    json['links'].append(link)
    return json