const ipcRenderer = require('electron').ipcRenderer;

const nodes = svg.select('.nodes'),
    links = svg.select('.links'),
    codeNames = svg.select('.codeNames');

ipcRenderer.on('resize', (event, size) => {
    const [width, height] = size;
    console.log(size);
    svg.attr('viewBox', `0 0 ${Math.pow(WIDTH, 2) / width} ${Math.pow(HEIGHT, 2) / height}`);
});