# bluetooth_psoc
Laboratorio 5 UART

Diseñar e implementar un sistema de almacenamiento de datos en una microSD con visualización en un Smartphone o en una Tablet.  El sistema tendrá los siguientes requerimientos:
•	Se podrán adquirir datos de por lo menos tres fuentes externas diferentes: sensor de temperatura, velocidad de un motor y voltaje.
•	Para la transferencia de datos entre el PSoC y el smartphone se hará uso del estándar bluetooth por medio de un módulo externo conectado al PSoC y el módulo interno del smartphone.
•	En el smartphone se deberá desarrollar una aplicación en Android que permita la interacción con el sistema, permitiendo seleccionar la fuente de datos a visualizar con su respectiva escala y sus unidades adecuadas.
•	El almacenamiento de datos se realizara en una microSD conectada al PSoC.

Operación general del sistema:
•	La aplicación en el Smartphone verificara inicialmente la conexión entre el PSoC y el teléfono
•	Luego se seleccionara alguna de las tres posibles fuentes de datos, el tiempo de la adquisición y el intervalo de tiempo entre muestras.
•	Al confirmar la selección se enviara algún tipo de comando al PSoC, por medio del bluetooth, donde se le indique la tarea a realizar.
•	El PSoC será el encargado de manejar la fuente de datos, el tiempo total, el tiempo entre muestras y el almacenamiento de los datos en la microSD
•	El mensaje enviado por el teléfono o la Tablet dará inicio al proceso de adquisición de datos,  almacenamiento de los mismos en la microSD y al envío por bluetooth al Smartphone o Tablet para que sean visualizados en una gráfica.
•	Al finalizar el tiempo de adquisición se cerrara el archivo en la microSD que deberá tener como nombre un identificador con información de la fuente de datos almacenada junto con un subíndice.
•	Se podrá también, basado en la información almacenada en la microSD, visualizar un archivo almacenado con anterioridad, borrar un archivo o borrar la memoria, todo esto controlado desde el Smartphone o la Tablet.
 


