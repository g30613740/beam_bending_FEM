<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Beam Bending Analysis — учебный проект</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        body {
            background: #f5f7fb;
            font-family: 'Segoe UI', Roboto, 'Helvetica Neue', sans-serif;
            line-height: 1.6;
            color: #1e2a3e;
            padding: 2rem 1rem;
        }
        .container {
            max-width: 1100px;
            margin: 0 auto;
            background: white;
            border-radius: 24px;
            box-shadow: 0 20px 35px -12px rgba(0,0,0,0.1);
            overflow: hidden;
            padding: 2rem 2rem 2.5rem;
        }
        h1 {
            font-size: 2.2rem;
            font-weight: 600;
            margin-bottom: 0.5rem;
            border-left: 5px solid #2c7da0;
            padding-left: 1rem;
            color: #0b3b4f;
        }
        .subhead {
            font-size: 1.1rem;
            color: #2c7da0;
            margin-bottom: 1.8rem;
            border-bottom: 1px solid #e2e8f0;
            padding-bottom: 0.75rem;
        }
        h2 {
            font-size: 1.6rem;
            margin: 1.5rem 0 1rem;
            color: #1f5068;
            font-weight: 500;
            border-bottom: 2px solid #cbd5e1;
            padding-bottom: 0.3rem;
        }
        h3 {
            font-size: 1.3rem;
            margin: 1.2rem 0 0.6rem;
            color: #2c3e50;
        }
        .badge {
            background: #eef2ff;
            border-radius: 40px;
            padding: 0.2rem 0.8rem;
            font-size: 0.8rem;
            font-weight: 500;
            color: #1e40af;
            display: inline-block;
            margin-bottom: 0.5rem;
        }
        pre {
            background: #1e293b;
            color: #e2e8f0;
            padding: 1rem 1.2rem;
            border-radius: 16px;
            overflow-x: auto;
            font-family: 'Fira Code', 'Cascadia Code', monospace;
            font-size: 0.85rem;
            margin: 1rem 0;
            box-shadow: inset 0 0 0 1px #334155;
        }
        code {
            background: #eef2ff;
            padding: 0.2rem 0.4rem;
            border-radius: 8px;
            font-family: monospace;
            font-size: 0.9em;
            color: #0c4a6e;
        }
        pre code {
            background: transparent;
            color: inherit;
            padding: 0;
        }
        .tree {
            background: #f8fafc;
            border-left: 4px solid #2c7da0;
            padding: 0.8rem 1.2rem;
            border-radius: 14px;
            font-family: monospace;
            font-size: 0.85rem;
            line-height: 1.5;
            white-space: pre-wrap;
        }
        .grid-2 {
            display: flex;
            flex-wrap: wrap;
            gap: 1.5rem;
            margin: 1.5rem 0;
        }
        .card {
            background: #f8fafc;
            border-radius: 20px;
            padding: 1.2rem 1.5rem;
            flex: 1;
            min-width: 200px;
            box-shadow: 0 2px 6px rgba(0,0,0,0.05);
            border: 1px solid #e2edf2;
        }
        .card h4 {
            font-size: 1.2rem;
            margin-bottom: 0.5rem;
            color: #1e3a5f;
        }
        hr {
            margin: 2rem 0;
            border: none;
            height: 1px;
            background: linear-gradient(to right, #cbd5e1, transparent);
        }
        footer {
            margin-top: 2rem;
            font-size: 0.85rem;
            text-align: center;
            color: #5b6e8c;
            border-top: 1px solid #e2e8f0;
            padding-top: 1.5rem;
        }
        a {
            color: #2c7da0;
            text-decoration: none;
        }
        a:hover {
            text-decoration: underline;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            margin: 1rem 0;
        }
        th, td {
            border: 1px solid #cbd5e1;
            padding: 0.5rem;
            text-align: left;
            vertical-align: top;
        }
        th {
            background: #eef2ff;
        }
    </style>
</head>
<body>
<div class="container">
    <h1> Beam Bending</h1>
    <div class="subhead">Учебный проект по расчёту изгиба балки</div>

    <p><strong>Анализ изгиба балки</strong> — классическая задача сопротивления материалов. В этом проекте балка длиной 25 условных единиц нагружена распределённой нагрузкой, сосредоточенными силами и моментами, опирается на шарнирные, жёсткие и упругую опору в заданных точках. Реализованы два независимых подхода:</p>
    <ul>
        <li> <strong>Аналитическое решение</strong> в системе SageMath (метод начальных параметров с функциями Хевисайда).</li>
        <li> <strong>Численное решение</strong> методом конечных элементов на C++ (с построением матрицы жёсткости и решением СЛАУ).</li>
    </ul>
    <p>Результаты сравниваются и визуализируются с помощью Python-скрипта.</p>

    <h2> Структура проекта</h2>
    <div class="tree">
        beam-bending-analysis/<br>
        ├── data/                     # Входные данные<br>
        │   ├── parameters.txt        # физические параметры (P, M, Q1, Q2, K, E, J)<br>
        │   ├── coordinates.txt       # координаты характерных точек<br>
        │   └── Балка_2025.xlsx       # схема нагружения (Excel)<br>
        ├── src/<br>
        │   ├── sage/                 # аналитическое решение в Sage<br>
        │   │   ├── beams_bending.sage<br>
        │   │   └── beams_bending.py<br>
        │   ├── fem/                  # метод конечных элементов на C++<br>
        │   │   └── beams_bending.cpp<br>
        │   └── plotting/             # визуализация результатов<br>
        │       └── paint.py<br>
        ├── results/                  # выходные данные расчётов<br>
        │   ├── sage_res_*.txt        # прогибы, углы, моменты, силы (Sage)<br>
        │   ├── FEM_res_*.txt         # аналогично для МКЭ<br>
        │   └── result_plot.png       # сравнение графиков<br>
        ├── parameters.txt → data/parameters.txt   # симлинк для удобства<br>
        ├── coordinates.txt → data/coordinates.txt<br>
        ├── README.md<br>
        ├── README.html<br>
        └── .gitignore<br>
    </div>

    <h2> Запуск и использование</h2>

    <div class="grid-2">
        <div class="card">
            <h4> Аналитическое решение (Sage)</h4>
            <pre><code>cd src/sage
sage beams_bending.sage</code></pre>
            <p>Результаты появляются в папке <code>results/</code> (относительно корня проекта).</p>
        </div>
        <div class="card">
            <h4> Численное решение (C++ FEM)</h4>
            <pre><code>cd src/fem
g++ beams_bending.cpp -o beams_bending
./beams_bending</code></pre>
            <p>Исполняемый файл должен видеть <code>parameters.txt</code> и <code>coordinates.txt</code> (симлинки в корне проекта обеспечивают доступ).</p>
        </div>
        <div class="card">
            <h4> Построение графиков</h4>
            <pre><code>cd src/plotting
python paint.py</code></pre>
            <p>Скрипт читает данные из папки <code>results/</code> и создаёт <code>result_plot.png</code>.</p>
        </div>
    </div>

    <h2> Требования</h2>
    <table>
        <tr><th>Компонент</th><th>Необходимое ПО / версии</th></tr>
        <tr><td>Sage-решение</td><td>SageMath ≥ 9.0 (поддерживает <code>desolve</code>, символьные вычисления)</td></tr>
        <tr><td>C++ FEM</td><td>Компилятор C++11 (g++, clang), стандартная библиотека</td></tr>
        <tr><td>Python-графика</td><td>Python 3, <code>matplotlib</code>, <code>numpy</code> (опционально, для paint.py). <br>При желании GSL для численных методов — необязательно.</td></tr>
    </table>

    <h2> Результаты</h2>
    <p>На выходе получаются текстовые файлы с массивами значений (координата x, прогиб W, угол поворота θ, момент M, поперечная сила Q) для обоих методов. Графическое сравнение позволяет оценить точность и сходимость МКЭ по отношению к аналитическому решению.</p>

    <h2> Лицензия</h2>
    <p>Проект распространяется под лицензией <strong>MIT</strong> (см. файл <code>LICENSE</code>). Учебные материалы могут быть свободно использованы и модифицированы.</p>

    <h2> Автор </h2>
    <p>@g30613740<br>
    </p>

    <hr>
</div>
</body>
</html>