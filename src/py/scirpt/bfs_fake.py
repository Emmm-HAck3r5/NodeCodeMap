def bfs_build(max_depth, root):
	nodes = [root]
	links = links.extend(create_links_with_all_children(root))
	queue = get_all_childs(root)
	for d in range(max_depth): # do max_depth times
		nodes.extend(queue)
		for node in queue:
			links = create_links_with_all_children(links, node)
		queue = [] # clean queue
		for node in queue:
			queue = add_children_to_queue(node, queue)
	return nodes, links
def create_links_with_all_children(root):
	links = []
	now_child = root
	next_child = root.rightchild
	while now_child != next_child:
		links.append(create_links(next_child, now_child))
		now_child = next_child
		next_child = next_child.rightchild
	return links
def add_children_to_queue(node, queue):
	now_child = node
    next_child = node.rightchild
	queue.append(now_child)
	while now_child != now_child.leftchild:
		queue.append(next_child)
		now_child = next_child
		next_child = next_child.rightchild
	return queue
def create_links(nodeone, nodetwo):
    return {
        "source": nodeone,
        "target": nodetwo
    }