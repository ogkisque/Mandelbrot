# Множество Мандельброта

## Что это такое

Это множество точек на комплексной плоскости, для которых выполняется:

|z_n| < R, z_n = z_(n-1) ^ 2 + z_0

То есть его можно рисовать программно, задав некоторую раскраску и выполняя следующий переход в цикле:

x_n = x_(n-1)^2 - y_(n-1)^2 + x_0
y_n = 2*x_(n-1)*y_(n-1) + y_0

В моем случае это выглядит следующим образом:

<p align = "center">
  <img src = "https://github.com/ogkisque/Mandelbrot/blob/master/Screenshot%20from%202024-03-25%2018.41.25.png" width = 500 height = 500>
</p>

### Расчёт времени и FPS

Целью этого проекта является изучение того, как оптимизации влияют на время исполнения программы. Поэтому нужно как-либо измерять это время.

Для расчёта FPS я вызываю функцию SDL_GetTicks дважды, вычитаю полученные значения и получаю таким образом время отрисовки 1 кадра в миллисекундах. Разделив 1000 на это значение, я получаю FPS.

Для расчёта времени выполнения лучше использовать более точные средства, чем SDL_GetTicks. Я использую функцию _rdtsc из <immitrin.h> (intrinsic функция). Она возвращает количество тактов с последнего сброса процессора. Аналогично вычитая два значения, я получу время отрисовки 1 кадра в тактах. Его мы и будем сравнивать.

## Отсутствие каких-либо оптимизаций

В этом базовом случае мы рассчитываем траекторию каждой точки плоскости последовательно. Получаются следующие результаты:

FPS = 3, ticks = 6969469

## Оптимизация -O3

Флаг -O3 позволяет компилятору делать самые сильные оптимизации, которые он может. Не изменяя программы, я скомпилировал с этим флагом и получил следующие результаты:

FPS = 7, ticks = 3358901

То есть FPS вырос в 2 раза, а время выполнения уменьшилось в 2 раза.

## Оптимизация через SIMD инструкции

SIMD (Single-Instruction, Multiple-Data) - это специальный набор инструкций, которые предоставляют параллельную (векторную) обработку данных. Я использовал инструкции AVX2, так как это максимальное, что поддерживает мой процессор.

С помощью этих инструкций я могу выполнять операцию одновременно с 8 числами (4-байтовыми), то есть можно обсчитывать траектории одновременно 8 точек плоскости. В результате получились следующие характеристики скорости:

FPS = 10, ticks = 2354491

То есть FPS вырос в 3 раза, а время выполнения уменьшилось в 3 раза.

## Оптимизация -O3 + AVX

Скомпилировав программу с AVX инструкциями с флагом -O3, я получил следующие результаты:

FPS = 50, ticks = 549538

То есть FPS больше в 17 раз, время выполнения меньше в 13 раз.

## Итоговое сравнение производительности

Измерив зависимость времени выполнения (в тактах) от размера картинки множества Мандельброта, я получил следующий график:

<p align = "center">
  <img src = "https://github.com/ogkisque/Mandelbrot/blob/master/image_2024-03-25_20-17-02.png" width = 500 height = 500>
</p>

Из него наглядно видно, как оптимизации ускоряют программу. Так же можно заметить, что все кривые отлично ложатся на квадратичную зависимость. Это как раз соответствует нашей программе (делается проход по всем пикселям, а их количество равно квадрату размера картинки).
