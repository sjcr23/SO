document.addEventListener("DOMContentLoaded", function () {
    
    let createButton = document.getElementById('createButton');
    let saveFile = document.getElementById('saveFile');

    
    if (createButton) {
        createButton.addEventListener('click', function() {
            const algSelector = document.getElementById('algSelector').value;
            const seedInput = document.getElementById('seedInput').value;
            const numProcess = document.getElementById('numProcess').value;
            const numInstructions = document.getElementById('numInstructions').value;
            
            const container = document.getElementById('generation')
            const msg = document.createElement('p');
            msg.style = "font-family: monospace;"
            msg.textContent = `File generated: {Algorithm:${algSelector}, Seed:${seedInput}, N° Processes:${numProcess}, N° Instructions:${numInstructions}}`
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
    }
    
    if (saveFile) {
        saveFile.addEventListener('click', function() {
            const fileForm = document.getElementById('fileForm')
            const msg = document.createElement('p');
            msg.style = "font-family: monospace;"
            msg.textContent = `File saved succesfully`
            fileForm.appendChild(msg)
        });
    }
    
});