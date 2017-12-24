
json_data = {
    'links': [],
    'nodes': []
}
json_data.nodes.append(root)
dfs_build(root)
def dfs_build(node):
    if(node.leftchild == node):
        return None
    json_data.nodes.append(create_node(node.leftchild), create_node(node.rightchild))
    leftlink = create_link(node, node.leftchild)
    rightlink = create_link(node, node.rightchild)
    json_data.links.append(leftlink, rightlink)
    dfs_build(node.leftchild)
    dfs_build(node.rightchild)
def create_link(node, child):
    return {
        "source": node.name,
        "target": child.name
    }
def create_node(node):
    return {
        "id": node.name
    }
