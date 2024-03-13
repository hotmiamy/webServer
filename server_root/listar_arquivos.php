<?php
$directory = '/server_root'; // Substitua pelo caminho do diretório que você deseja listar
$fileList = array_diff(scandir($directory), array('..', '.')); // Lista os arquivos, excluindo . e ..

header('Content-Type: application/json');
echo json_encode($fileList);
?>