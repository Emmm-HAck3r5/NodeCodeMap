const WIDTH = Math.min(window.screen.width, document.documentElement.clientWidth);
const HEIGHT = Math.min(window.screen.height, document.documentElement.clientHeight) - 20; // give up thinking.jpg
const centerX = WIDTH / 2;
const centerY  = HEIGHT / 2;

let codeNameToggleSwitch = false;
let toggle = true;

let svg = d3.select('svg')
    .attr('width', WIDTH)
    .attr('height', HEIGHT)
    .on('dblclick', dblclicked);

let simulation = d3.forceSimulation()
    .force('charge', d3.forceManyBody().strength(1.2))
    .force('center', d3.forceCenter(centerX, centerY))
    .force('collide', d3.forceCollide(1))
    .velocityDecay(0);
let color = d3.scaleOrdinal(d3.schemeCategory20);

let dLinks, dNodes;

d3.json('../public/fortest.json', treeInit);

function dblclicked(){
    const nodes = svg.selectAll('circle');
    const links = svg.selectAll('line');
    const codeNames = svg.selectAll('text');
    
    if (toggle ){
        setNetSimulation(nodes, links, codeNames);
        toggle = false;
    } else{
        removeNetSimulation(nodes, links, codeNames);
        toggle = true;
    }
}

function treeInit(err, data){
    if(err){
        console.log(err);
    }
    let dataNodes = data.nodes.map( d => {
        const theta = Math.random() * 2 * Math.PI;
        const r = HEIGHT / 2 * Math.random();
        const x = centerX + r * Math.cos(theta);
        const y = centerY + r * Math.sin(theta);
        return {
            id: d.id,
            x: x,
            y: y,
            vx: (centerY - y) * 0.05,
            vy: (x - centerX) * 0.05,
            value: d.value
        };
    });
    let links = svg.append('g')
        .classed('links', true)
        .selectAll('line')
        .data(data.links)
        .enter()
        .append('line')
            .attr('class', (d, i) => `LINE-${i}`)
            .attr('stroke-width', () => 3)
            .attr('visibility', 'hidden');
    let nodes = svg.append('g')
        .classed('nodes', true)
        .selectAll('circle')
        .data(dataNodes)
        .enter()
        .append('circle')
            .attr('id', (d) => `NODE-${d.id}`)
            .attr('r', (d) => d.value)
            .attr('fill', (d) => color(d.x))
            .attr('cx', (d) => d.x)
            .attr('cy', (d) => d.y);
    let codeNames = svg.append('g')
        .classed('codeNames', true)
        .selectAll('text')
        .data(dataNodes)
        .enter()
        .append('text')
            .attr('id', (d) => `TEXT-${d.id}`)
            .text((d) => d.id)
            .attr('text-anchor', 'middle')
            .attr('visibility', 'hidden');
    simulation.nodes(dataNodes).alphaDecay(0)
        .on('tick', () => {
            nodes
                .attr('cx', (d) => d.x + d.vx * 3)
                .attr('cy', (d) => (d.y + d.vy * 3));
            codeNames
                .attr('x', (d) => d.x + d.vx * 3)
                .attr('y', (d) => (d.y + d.vy * 3 + getFontSize(d.id) / 4));
        });
    addDragEeventListener(nodes, codeNames);
    addZoomEventListener(nodes, codeNames, links);

    dLinks = data.links;
    dNodes = dataNodes;
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
            .alpha(1)
            .alphaTarget(0);
    }
    d.fx = null;
    d.fy = null;
}

// text toggle
function textToggle(){
    if(codeNameToggleSwitch === false){
        codeNameToggleSwitch = true;
    }else{
        codeNameToggleSwitch = false;
    }
    return codeNameToggleSwitch;
}
// 
function addZoomEventListener(nodes, codeNames, links){
    svg.call(d3.zoom()
        .scaleExtent([0.1,10])
        .on('zoom', () => {
            const t = d3.event.transform; // t is d3.event.transform for short
            const transformSTYLE = `translate(${t.x}, ${t.y}) scale(${t.k})`;
            for(const selection of [nodes, codeNames, links]){
                zoomTransition(selection, transformSTYLE);
            }
        }))
        .on('dblclick.zoom', null);
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

// // each event listeners
function addDragEeventListener(nodes, codeNames){
    nodes
        .call(d3.drag()
            .on('start', drags)
            .on('drag', dragging)
            .on('end', dragged));
    codeNames
        .call(d3.drag()
            .on('start', drags)
            .on('drag', dragging)
            .on('end', dragged));
}


// set data (nodes, links) to simulation
function setNetSimulation(nodes, links, codeNames){
    links.attr('visibility', 'visible');
    codeNames.attr('visibility', 'visible');
    simulation
        .force('charge', d3.forceManyBody())
        .force('link', d3.forceLink()
            .links(dLinks)
            .id( (d) => d.id )
            .distance(75))
        .force('collide', d3.forceCollide(10))
        .velocityDecay(0.05);
    
    simulation.nodes(dNodes).alphaDecay(0)
        .on('tick', () => {
            nodes
                .attr('cx', d => d.x)
                .attr('cy', d => d.y);
            links
                .attr('x1', (d) => d.source.x)
                .attr('y1', (d) => d.source.y)
                .attr('x2', (d) => d.target.x)
                .attr('y2', (d) => d.target.y);
            codeNames
                .attr('x', (d) => d.x)
                .attr('y', (d) => d.y + getFontSize(d.id) / 4);
        });
    return simulation;
}
function removeNetSimulation(nodes, links, codeNames){
    links.attr('visibility', 'hidden');
    codeNames.attr('visibility', 'hidden');
    simulation
        .force('link', null)
        .force('charge', d3.forceManyBody().strength(1.2))
        .force('center', d3.forceCenter(centerX, centerY))
        .force('collide', d3.forceCollide(1))
        .velocityDecay(0);
    simulation.nodes(dNodes).alphaDecay(0)
        .on('tick', () => {
            nodes
                .attr('cx', (d) => d.x + d.vx * 3)
                .attr('cy', (d) => (d.y + d.vy * 3));
            codeNames
                .attr('x', (d) => d.x + d.vx * 3)
                .attr('y', (d) => (d.y + d.vy * 3 + getFontSize(d.id) / 4));
        });
}