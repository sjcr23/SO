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


async function update_mem(pc, index) {
    const real_memory = pc.real_memory
    const columns = document.getElementById('tableContainer').getElementsByClassName('column');
    const table = columns[index].getElementsByTagName('table')[0];

    real_memory.forEach(async state => {
        // Start always at same color
        currentColor = 0;
        let color = getColor();
        let lastNumber = state[0];
        
        
        for (let i = 0; i < state.length; i++) {
            if (lastNumber != state[i]) {
                color = getColor();
                lastNumber = state[i];
            }
            changeCellColor(table, 1, i, color);
        }

    });
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
                const memory = data.memory_state;
                console.log(memory)
                const PC0 = memory.PC0
                const PC1 = memory.PC1
                update_mem(PC0, 0);
                update_mem(PC1, 1);
            });
        });
    }   
    
    if (pauseButton) {
        pauseButton.addEventListener('click', function() {
            fetch('/get-color')
            .then(response => response.json())
            .then(async data => {
                // Acceder a todas las columnas dentro del contenedor 'tableContainer'
                const columns = document.getElementById('otherTablesContainer').getElementsByClassName('column');
                const table0 = columns[0].getElementsByTagName('table')[0];
                const table1 = columns[1].getElementsByTagName('table')[0];
                console.log(table0);
                console.log(table1);
                
                
                const memory = data.memory_state;
                const PC0 = memory.PC0
                const PC1 = memory.PC1
                update_mem(PC0, 0);
                update_mem(PC1, 1);
            });
            
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


