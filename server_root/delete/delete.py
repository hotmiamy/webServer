#!/usr/bin/env python
import os
import json

# Pasta onde os arquivos serão listados
FILE_DIRECTORY = '/home/llopes-n/42/projetos/webserver/server_root/delete/files/'

# Lista os arquivos na pasta especificada
files = os.listdir(FILE_DIRECTORY)

# Retorna a lista de arquivos como uma resposta JSON
print(json.dumps(files))