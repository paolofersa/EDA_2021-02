EL codigo principal se encuentra en la direccion:
	/source/main.cpp

Los datos utilizados fueron extraidos de la siguiente página de KAGGLE: https://www.kaggle.com/c/digit-recognizer/data
Estos fueron descargados y se encuentran en la carpeta: /resources, con los nombres "test.csv" y "train.csv" correspondientemente.
De igual forma los archivos se encuentran almacenados en Google Drive en las siguientes direcciones (descargarlos en caso no se tengan ya dentro de la carpeta: /resources)
	https://drive.google.com/file/d/1Ei0rxzB7tBELhstET88EYx4haaWxy5-M/view?usp=sharing
	https://drive.google.com/file/d/136GJFcJxKXCfXctwwGz0dToI4wnah45W/view?usp=sharing

Para la ejecución adecuada del programa:
 (1) Verificar que se encuentren los archivos "test.csv" y "train.csv" dentro del directorio /resources
   (1.1) En caso no estén, descargar los archivos "test.csv" y "train.csv" de los enlaces anteriores y colocarlos en directorio: /resources
 (2) Crear directorio vacío /build (para ejecutar CMake)
 (3) Ejecutar el archivo CMakeLists.txt

Consideraciones del programa:
   - 	El código insertará automáticamente 70 000 elementos en la estructura "TertiaryHashTree"
	de los conjuntos de datos test y train (se visualizará la cantidad de elementos insertados y el tiempo utilizado).
   -	Se almacenarán los valores de los elementos del conjunto de datos test (28 000 elementos) en un vector (vector<string> elementsForQuery)
	independiente de la estructura de datos que será utilizado como valores para las consultas.
   -	Se solicitará al usuario el número de elemento (id) para la consulta K-NN de entre los 28 000 elementos del conjunto de datos test.
   -	Se solicitará al usuario el valor de k para la consulta K-NN.
   -	Se visualizarán el tiempo de la consulta y la recuperación de los datos, así como todos los elementos recuperados
	en un vector de respuesta (vector<ImageStructure<string>> answer). Cada elemento mostrará el número de imagen, a qué conjunto de datos pertenece (train, test),
	y la etiqueta de número (0-9) en caso la tenga (para conjuntos de datos de train).


*TODOS LOS ARCHIVOS COMPLETOS CON EL CMAKE LISTO PARA EJECUTAR SE ENCUENTRAN EN EL SIGUIENTE REPOSITORIO DE GITHUB:
https://github.com/paolofersa/EDA_2021-02/tree/main/Tertiary_Hash_Tree