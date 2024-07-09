% Definir el rango de x
x = logspace(-1, 3, 100); % de 0.1 a 1000 en una escala logarítmica

% Calcular y usando la función dada
y = 20/3 * x;

% Crear la gráfica en escala logarítmica
figure;
loglog(x, y, 'LineWidth', 2);
grid on;

% Etiquetas de los ejes
xlabel('Concentration [ppm]');
ylabel('Rs/R0');

% Título del gráfico
title('Gráfica de y = 20/3*x en escala logarítmica');

% Mostrar la gráfica
legend('y = 20/3*x');
