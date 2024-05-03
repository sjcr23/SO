// // Crea en HTML la tabla, con nombre
// function createMemoryTable(name) {
//     const container = document.getElementById('tableContainer');
//     const title = document.createElement('div');
//     title.className = 'table-title';
//     title.textContent = name;
//     container.appendChild(title);

//     const table = document.createElement('table');
//     const tbody = document.createElement('tbody');
//     table.appendChild(tbody);

//     const row = tbody.insertRow();
//     for (let j = 1; j <= 100; j++) {
//         const cell = row.insertCell();
//         cell.textContent = '';
//     }
//     container.appendChild(table);
//     return table;
// }

// // Actualiza la función de cambio de color para aceptar el objeto de tabla
// function changeCellColor(table, row, col, color) {
//     if (row >= 1 && row <= table.rows.length && col >= 1 && col <= table.rows[row - 1].cells.length) {
//         const cell = table.rows[row - 1].cells[col - 1];
//         cell.style.backgroundColor = color;
//     }
// }

// function sleep(ms) {
//     return new Promise(resolve => setTimeout(resolve, ms));
// }


// async function fill_memory(item, color) {
//     for (let i = 24; i <=43; i++) {
//         await sleep(45);
//         changeCellColor(item, 1, i, color);
//     }
// }

// async function free_memory(item) {
//     for (let i = 100; i >= 0; i--) {
//         await sleep(45);
//         changeCellColor(item, 1, i, "white");
//     }
// }

// document.addEventListener("DOMContentLoaded", function () {
//     const table1 = createMemoryTable('RAM - [OPT]');
//     const algSelector = document.getElementById('algSelector');
//     var created = false;
//     var tables = [table1]

//     const createButton = document.getElementById('createButton');
//     createButton.addEventListener('click', function() {
//         if (!created){
//             const table2 = createMemoryTable(`RAM - [${algSelector.value}]`);
//             tables.push(table2)
//             created= true;
//         }
//     });
// });


document.getElementById('createButton').addEventListener('click', function() {
    const algSelector = document.getElementById('algSelector').value;
    const seedInput = document.getElementById('seedInput').value;
    const numProcess = document.getElementById('numProcess').value;
    const numInstructions = document.getElementById('numInstructions').value;
    
    const container = document.getElementById('generation')
    const msg = document.createElement('p');
    msg.style = "font-family: monospace;"
    msg.textContent = `File generated: {alg:${algSelector}, seed:${seedInput}, P:${numProcess}, N:${numInstructions}}`
    container.appendChild(msg)
    const data = {
        algorithm: algSelector,
        seed: seedInput,
        processes: numProcess,
        instructions: numInstructions
    };

    fetch('/setup', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(data)
    })
    .then(response => response.json())
    .then(data => {
        console.log('Success:', data);
    })
    .catch((error) => {
        console.error('Error:', error);
    });
});