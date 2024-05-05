from flask import Flask, render_template, request, jsonify, send_file, abort
import os

app = Flask(__name__)

file_path = './out/output.txt'

def generate_file(data):
    algorithm, seed, processes, instructions = data
    with open(file_path, 'w') as file:
        file.write(algorithm + "\n")
        file.write(seed + "\n")
        file.write(processes + "\n")
        file.write(instructions + "\n")
    file.close()
    return f'File generated successfully in {file_path}'

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

@app.route('/simulation')
def simulation():
    return render_template('simulation.html')


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


if __name__ == '__main__':
    app.run(debug=True)