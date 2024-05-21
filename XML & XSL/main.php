<?php
//Загружаем XML-файл
$xml = new DOMDocument();
$xml->load('books.xml');

//Загружаем XSL-файл
$xsl = new DOMDocument();
$xsl->load('books.xsl');

//Создаём процессор и импортируем таблицу стилей XSL
$proc = new XSLTProcessor();
$proc->importStylesheet($xsl);

//Получаем выбранный столбец для сортировки и название для поиска книги,
$sort_by = isset($_GET['sort_by']) ? $_GET['sort_by'] : 'title';
$book_title = isset($_GET['book_title']) ? $_GET['book_title'] : '';

//Создаем параметры для преобразования XSL
$proc->setParameter(null, 'sort_by', $sort_by);
$proc->setParameter(null, 'book_title', $book_title);

//Применяем преобразования XSL к файлу XML
$html = $proc->transformToXML($xml);

echo $html;
?>

<br>
<form>
    <label for="sort_by">Выберите столбец для сортировки:</label>
    <select name="sort_by">
        <option value="title">Название</option>
        <option value="author">Автор</option>
        <option value="genre">Жанр</option>
        <option value="year">Год издания</option>
        <option value="price">Цена</option>
    </select>
    <input type="submit" value="Сортировать">
</form>

<form method="get">
    <label for="search_title">Введите название книги:</label>
    <input type="text" id="book_title" name="book_title">
    <input type="submit" value="Найти">
</form>