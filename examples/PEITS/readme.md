# PEITS example usage

[PEITSADASDASDASDAd](PEITSASDASD) is a parallel EIT solver designed for high precision computation of the forward model in FEMs with a large number of elements. Both PEITS and MESHER run on Linux, as well as WSL. PEITS requires some specific files and parameters to be set in order to use the output from the MESHER, which are detailed here. The function `PEITS_example.m` also demonstrates how to use the MESHER and PEITS wrapper functions together for a simplified workflow

## 1. Run the Mesher

Here we are using the neonate skull as an example

```


```

## 2. Prepare MESHER files

PEITS has numerous parameters, but MESHER assumes the simplest use case:

- Electrode nodes are chosen by PEITS and *not* specified directly
- Conductivies are specified by a separate `conductivities` file, and the *indicies* written to the `.dgf` file

Most of these settings are specified in the `MESHNAME.parameters`file written by the MESHER. However we will need to create the conductivities file.

The neonate mesh contains two layers, the scalp and skull so we need to write one per line:

```bash

```


The file `PEITSex.conductivies` now contains:
```bash
0.2
0.03
```

## 3. Setup PEITS

PEITS has a few idiosyncrasies that we need to account for. First it is easiest if the inputs are stored in the `PEITS/data` folder. Second that the same parameter file is read each time, so the contents must be overwritten each time.

```
move files

Backup parameterfile

```

The important parameters in this file are

parameter
```
conductivity file
protocol
mesh
```

standard_params
```
current
diameter
```



## 4. Run PEITS

