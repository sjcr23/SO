from flask import Flask, render_template, request, jsonify, send_file, abort
import os 
from simulation import models
from simulation import utils

app = Flask(__name__)

folder = './out'
if not os.path.exists(folder):
    os.makedirs(folder)


# Define una carpeta para guardar los archivos descarga
app.config['UPLOAD_FOLDER'] = folder
default_input_name = os.path.join(app.config['UPLOAD_FOLDER'], 'input.txt')
file_path = os.path.join(app.config['UPLOAD_FOLDER'], "output.txt")

colors = []
mem_state = []

def generate_file(data):
    algorithm, seed, processes, instructions = data
    with open(file_path, 'w') as file:
        file.write(algorithm + "\n")
        file.write(seed + "\n")
        file.write(processes + "\n")
        file.write(instructions + "\n")
    file.close()



@app.route('/')
def home():
    return setup()



@app.route('/setup', methods=['GET', 'POST'])
def setup():
    if os.path.exists(file_path):
        os.remove(file_path)

    match request.method:
        case 'POST':
            data = request.get_json()
            generate_file(data.values())
            return jsonify({"message": "Datos recibidos exitosamente", "status": "success"})
        case 'GET':
            return render_template('setup.html')



@app.route('/get-color')
def get_color():
    global mem_state
    data = {
        'PC0':mem_state[0],
        'PC1':mem_state[1],
    }
    return jsonify(memory_state=data)



@app.route('/simulation')
def simulation():
    global mem_state
    global colors
    
    init_state = [{'cells':100*[" "]}]
    algs = [{'NAME': 'OPT'}]

    if os.path.exists(default_input_name):
        with open(default_input_name, 'r+') as file:
            content = file.readlines()
            algs.append({'NAME': content[0][:-1]})
        file.close()
    else:
        algs.append({'NAME': 'OPT'})

    OPT, ALG = utils.get_algorithms(algs)

    PC0 = models.Computer()
    PC1 = models.Computer(algorithm=ALG)

    PC0_mem_state = utils.excecute_file(PC0.mmu)
    PC1_mem_state = utils.excecute_file(PC1.mmu)
    mem_state = [PC0_mem_state, PC1_mem_state]
    
    return render_template('simulation.html', algorithms=algs, mem=init_state)



@app.route('/download-file')
def download_file():
    if not os.path.exists(file_path):
        # Si el archivo no existe, puedes retornar un mensaje de error
        # o redirigir al usuario a otra página con información adecuada
        abort(404, description="File not found.")  # Aborts with a 404 error

    return send_file(file_path, as_attachment=True, download_name='data.txt')



@app.errorhandler(404)
def page_not_found(e):
    # Tu lógica personalizada para mostrar una página de error
    return render_template('404.html', description=e.description), 404



@app.route('/upload-file', methods=['POST'])
def upload_file():

    if 'file' not in request.files:
        return home()
    
    file = request.files['file']
    if file.filename == '':
        abort(404, description="File not found.")
    
    if request.files['file']:
        request.files['file'].save(default_input_name)
        return simulation()
    return home()



if __name__ == '__main__':
    app.run(debug=True)