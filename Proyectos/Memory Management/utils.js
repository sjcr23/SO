// Crea en HTML la tabla, con nombre
export function createMemoryTable(name) {
    const container = document.getElementById('tableContainer');
    const title = document.createElement('div');
    title.className = 'table-title';
    title.textContent = name;
    container.appendChild(title);

    const table = document.createElement('table');
    const tbody = document.createElement('tbody');
    table.appendChild(tbody);

    for (let i = 0; i < 1; i++) { // Un solo ciclo para una fila
        const row = tbody.insertRow();
        for (let j = 1; j <= 100; j++) {
            const cell = row.insertCell();
            cell.textContent = '';
        }
    }
    container.appendChild(table);
}


// Actualiza la función de cambio de color para aceptar el objeto de tabla
export function changeCellColor(table, row, col, color) {
    if (row >= 1 && row <= table.rows.length && col >= 1 && col <= table.rows[row - 1].cells.length) {
        const cell = table.rows[row - 1].cells[col - 1]; // Accede a la celda correcta
        cell.style.backgroundColor = color; // Cambia el color de la celda
    }
}