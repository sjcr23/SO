var currentColor = 0;
var COLORS = ['aqua', 'crimson', 'darkGreen', 'blueViolet', 'aquamarine', 'DarkSlateBlue', 'goldenRod', 'hotPink', 'greenYellow', 'DodgerBlue', 'OrangeRed'];

function getColor() {
    const color = COLORS[currentColor % COLORS.length];
    currentColor++;
    return color;
}


function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}


function changeCellColor(table, row, col, color) {
    if (row >= 1 && row <= table.rows.length && col >= 0 && col <= table.rows[row - 1].cells.length) {
        const cell = table.rows[row - 1].cells[col];
        cell.style.backgroundColor = color;
    }
}

async function free_memory(item) {
    for (let i = 99; i >= 0; i--) {
        changeCellColor(item, 1, i, "white");
    }
}

function colorRAM(memory, state){
    currentColor = 0;
    let color = getColor();
    let lastNumber = state[0];
    
    for (let i = 0; i < state.length; i++) {
        if (lastNumber !== state[i]) {
            color = getColor();
            lastNumber = state[i];
        }
        changeCellColor(memory, 1, i, color); // Cambia el color de la celda especificada
    }
}

function colorTableRows(table, state){
    const tbody = table.getElementsByTagName('tbody')[0]; 

    for (let i = 0; i < state.length; i++) {
        const color = getColor();  // Obtiene un nuevo color para cada fila
        const tableRow = document.createElement('tr');
        
        // Creación y asignación de celdas dentro de la fila
        for (let j = 0; j < 8; j++) {  // Suponiendo que cada fila debería tener 8 celdas
            const cell = document.createElement('td');
            cell.textContent = state[i][j]; // Asigna el valor correspondiente de 'state'
            cell.style.backgroundColor = color; // Asigna el color
            tableRow.appendChild(cell);
        }

        tbody.appendChild(tableRow);
    }
}

async function update_mem(pc, index) {
    
    // RAM containers
    const memContainer = document.getElementById('tableContainer').getElementsByClassName('column');
    const ram = memContainer[index].getElementsByTagName('table')[0];

    
    // The other tables
    const contenedor = document.getElementById('otherTablesContainer');
    const tables = contenedor.getElementsByTagName('table');
    
    const memTable = tables[0];
    const prcTable = tables[1];
    const ramTable = tables[2];
    const trsTable = tables[3];
        
    console.log(memTable)

    // Iterar sobre cada estado en la memoria real
    for (let i = 0; i < pc.real_memory.length; i++) {
        const realMemoryState = pc.real_memory[i];
        const pointerMapState = pc.pointer_map[i];
        
        // Reiniciar siempre al mismo color
        colorRAM(ram, realMemoryState)
        colorTableRows(memTable, pointerMapState);

        await sleep(100);
    }
}

document.addEventListener("DOMContentLoaded", function () {

    let playButton = document.getElementById('playButton');
    let pauseButton = document.getElementById('pauseButton');
    let restartButton = document.getElementById('restartButton');


    if (playButton) {
        playButton.addEventListener('click', function() {
            fetch('/get-color')
            .then(response => response.json())
            .then(async data => {
                const simulation = data.memory_state;
        
                const PC0 = simulation.PC0
                const PC1 = simulation.PC1
                console.log(PC0)
                update_mem(PC0, 0);
                update_mem(PC1, 1);
            });
        });
    }   
    
    if (pauseButton) {
        pauseButton.addEventListener('click', function() {
            // Acceder a todas las columnas dentro del contenedor 'tableContainer'
            const columns = document.getElementById('tableContainer').getElementsByClassName('column');
            const tables = columns[1].getElementsByTagName('table');
            fill_memory(tables[0], 'red');
        });
    }
    
       
    if (restartButton) {
        restartButton.addEventListener('click', function() {
            // Acceder a todas las columnas dentro del contenedor 'tableContainer'
            const columns = document.getElementById('tableContainer').getElementsByClassName('column');
            const table0 = columns[0].getElementsByTagName('table')[0];
            const table1 = columns[1].getElementsByTagName('table')[0];
            free_memory(table0);
            free_memory(table1);
        });
    }
    
});


