from flask import Flask, render_template, request, jsonify

app = Flask(__name__)



@app.route('/')
def home():
    return setup()

@app.route('/setup', methods=['GET', 'POST'])
def setup():
    match request.method:
        case 'POST':
            data = request.get_json()
            print(data)
            return jsonify({"message": "Datos recibidos exitosamente", "status": "success"})
        case 'GET':
            return render_template('setup.html')

@app.route('/simulation')
def simulation():
    return render_template('simulation.html')

if __name__ == '__main__':
    app.run(debug=True)