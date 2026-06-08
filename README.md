<!DOCTYPE html>
<html>
<body>
<h1> Учебный проект по расчёту изгиба балки (дорабатывается)</h1> 

<p><strong>Анализ изгиба балки</strong> — классическая задача сопротивления материалов. В проекте балка длиной 25 условных единиц нагружена распределённой нагрузкой, сосредоточенными силами и моментами, опирается на шарнирные, жёсткие и упругую опору в заданных точках. Реализованы два независимых подхода:</p>
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