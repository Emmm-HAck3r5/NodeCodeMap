var ipcRenderer = require('electron').ipcRenderer;

const nodes = svg.select('.nodes'),
    links = svg.select('.links'),
    codeNames = svg.select('.codeNames');

ipcRenderer.on('resize', (event, size) => {
    const [width, height] = size;
    svg.attr('viewBox', `0 0 ${Math.pow(WIDTH, 2) / width} ${Math.pow(HEIGHT, 2) / height}`);
});


ipcRenderer.on('maximize', (event, size) => {
    const [width, height] = size;
    svg.attr('width', window.screen.width)
        .attr('height', window.screen.height)
        .attr('viewBox', `0 0 ${window.screen.width} ${window.screen.height}`);
});

ipcRenderer.on('pythonDone', (event, jsonfile) => {
    console.log('PYTHON DONE! by ipcRenderer');
    console.log(jsonfile);
    D3Svg.initSvg(jsonfile);
    $('.searchbox').attr('style', 'display: none;');
});

let ipcFunc = {};

ipcFunc.getWinSize = function () {
    const size = ipcRenderer.sendSync('getWinSize');
    return size;
}