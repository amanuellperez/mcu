Este programa es para experimentar con los control remotos de infrared.

## TODO list
De momento paro el programa, está sin acabar. Cosas que se pueden mejorar:

1. Eliminar `Clock_us` a favor de la clase genérica `dev::Miniclock_us`

2. Las opciones de `research` y `clone` y `send` realmente se pueden fusionar
   en una. Están separadas porque es un programa de aprendizaje. ¿Merece la
   pena fusionarlas o dejarlas separadas?

3. No tengo ningún mando RC5, por eso no está implementado este protocolo.

4. Los archivos `dev_` son genéricos, no perteneciendo al proyecto. Hay que
   revisar si realmente son genéricos y si merece la pena llevarlos a `dev`.
