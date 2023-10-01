# FAT specification


## Bibliografía

* [Master Boot Record](https://en.wikipedia.org/wiki/Master_boot_record)
* [Boot sector](https://en.wikipedia.org/wiki/Boot_sector)

## Estructura del disco

### Boot sector

El primer sector del disco, el sector número 0, es el ` boot sector`. Voy a
dar por supuesto que en la tarjeta hay varias particiones con lo que este
sector contendrá toda la información de las particiones. En este caso se llama
`Master Boot Record`.

Los dos últimos bytes del primer sector (sector número 0) son `0x55 0xAA`
(boot sector signature). 

### Master Boot Record (MBR)

Según la wikipedia el formato de la MBR es:

| Address | Description | Size (bytes) |
|:-------:|:------------|:------------:|
|0x01BE (446) | Partición nº 1 | 16 |
|0x01CE (462) | Partición nº 2 | 16 |
|0x01DE (478) | Partición nº 3 | 16 |
|0x01EE (494) | Partición nº 4 | 16 |
|0x01FE (510) | 0x55 0xAA | 2 |

Observar que solo se pueden definir 4 particiones, que los dos últimos bytes
son la signatura y que el tamaño del sector son 512 bytes.

#### Datos de la partición

Cada entrada para la partición son 16 bytes. De estos 16 bytes los que nos
interesan son:

| Offset (bytes) | Size (bytes) | Description |
| :-------------:|:------------:|:------------|
| 0x08           | 4  | Número de sector en el que empieza la partición |
| 0x0C           | 4  | Número de sectores de la partición |

Esta tabla está en little-endian.


##### Ejemplo

Usando el SD inspector leer el sector 0. Confirmar que los dos últimos bytes
de este sector son 0x55 y 0xAA.

Después leer los 16 bytes del sector 0 a partir de la dirección 446. Los 4
bytes [8, 12) (empezando a contar por 0) dan el número de sector de la primera
partición (en little-endian). Cargar ese sector para ver que realmente es la
primera partición.




