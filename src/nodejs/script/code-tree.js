
let WIDTH = Math.min(window.screen.width, document.documentElement.clientWidth);
let HEIGHT = Math.min(window.screen.height, document.documentElement.clientHeight) - 20; // give up thinking.jpg

let svg = d3.select('svg')
    .attr('width', WIDTH)
    .attr('height', HEIGHT);

let simulation = d3.forceSimulation()
    .force('charge', d3.forceManyBody())
    .force('center', d3.forceCenter(WIDTH / 2, HEIGHT / 2))
    .force('collide', d3.forceCollide(30));

d3.json('../public/codetree-test.json', treeInit);

// initialize code tree
function treeInit(err, data){
    if(err){
        console.log(err);
    }
    // initialize elements
    let links = svg.append('g')
        .classed('links', true)
        .selectAll('line')
        .data(data.links)
        .enter()
        .append('line')
            .attr('stroke-width', () => 3);
    let nodes = svg.append('g')
        .classed('nodes', true)
        .selectAll('circle')
        .data(data.nodes)
        .enter()
        .append('circle')
            .attr('r', () => 30);
    let codeNames = svg.append('g')
        .classed('codeNames', true)
        .selectAll('text')
        .data(data.nodes)
        .enter()
        .append('text')
            .attr('id', (d) => `TEXT-${d.id}`)
            .text((d) => d.id)
            .attr('text-anchor', 'middle')

    setDataToSimulation(data, links, nodes, codeNames);

    addEventListenerToAllElements(links, nodes, codeNames);
}

// each event listeners of every elements
function addEventListenerToAllElements(links, nodes, codeNames){
    // There is no event to links
    // ...
    
    nodes.on('dblclick', centralize)
        .call(d3.drag()
            .on('start', drags)
            .on('drag', dragging)
            .on('end', dragged));
    codeNames.on('dblclick', centralize)
        .call(d3.drag()
            .on('start', drags)
            .on('drag', dragging)
            .on('end', dragged));

    svg.call(d3.zoom()
            .scaleExtent([0.5,10])
            .on('zoom', () => {
                const t = d3.event.transform; // t is d3.event.transform for short
                const transformSTYLE = `translate(${t.x}, ${t.y}) scale(${t.k})`;
                for(const selection of [links, nodes, codeNames]){
                    zoomTransition(selection, transformSTYLE);
                }
            }))
        .on('dblclick.zoom', null);
}


// set data (nodes, links) to simulation
function setDataToSimulation(data, links, nodes, codeNames){
    simulation
        .nodes(data.nodes)
        .on('tick', () => {
            links
                .attr('x1', (d) => d.source.x)
                .attr('y1', (d) => d.source.y)
                .attr('x2', (d) => d.target.x)
                .attr('y2', (d) => d.target.y);
            nodes
                .attr('cx', (d) => d.x)
                .attr('cy', (d) => d.y);
            codeNames
                .attr('x', (d) => d.x)
                .attr('y', (d) => d.y + getFontSize(d.id) / 4);
        });
    simulation.force('link',
        d3.forceLink()
            .links(data.links)
            .id( (d) => d.id )
            .distance(150));
    return simulation;
}


// the way of transition during zooming
function zoomTransition(selection, traSTY){
    selection.transition()
        .duration(400)
        .ease(d3.easeSinOut)
        .attr('transform', traSTY);
    return selection;
}


// get Text Element's height
function getFontSize(id){
    return document.getElementById(`TEXT-${id}`).getBBox().height;
}

// make node move to center
function centralize(d){
    console.log("uncompleted");
}

// drag listener
function drags(d){
    if(!d3.event.active){
        simulation.alphaTarget(0.3).restart();
    }
    d.fx = d.x;
    d.fy = d.y;
}
function dragging(d){
    d.fx = d3.event.x;
    d.fy = d3.event.y;
}
function dragged(d){
    if(!d3.event.active){
        simulation
            .alpha(1.5)
            .alphaTarget(0);
    }
    d.fx = null;
    d.fy = null;
}