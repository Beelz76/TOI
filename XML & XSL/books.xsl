<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	
  <xsl:param name="book_title"/>

	<xsl:template match="/">
		<html>
			<head>
				<title>Книги</title>
			</head>
			<body>
				<xsl:choose>
					<xsl:when test="$book_title != ''">
						<xsl:apply-templates select="books/book[contains(translate(title, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnopqrstuvwxyz'), translate($book_title, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnopqrstuvwxyz'))]"/>
					</xsl:when>
					<xsl:otherwise>
						<h1>Список книг</h1>
						<table border="1">
							<tr>
								<th>Название</th>
								<th>Автор</th>
								<th>Жанр</th>
								<th>Год издания</th>
								<th>Цена</th>
							</tr>
							<xsl:for-each select="books/book">
								<xsl:sort select="*[name()=$sort_by]"/>
								<tr>
									<td><xsl:value-of select="title"/></td>
									<td><xsl:value-of select="author"/></td>
									<td><xsl:value-of select="genre"/></td>
									<td><xsl:value-of select="year"/></td>
									<td><xsl:value-of select="price"/></td>
								</tr>
							</xsl:for-each>
						</table>
					</xsl:otherwise>
				</xsl:choose>
			</body>
		</html>
	</xsl:template>

	<xsl:template match="book">
		<h1>Результат поиска</h1>
		<table border="1">
			<tr>
				<th>Название</th>
				<th>Автор</th>
				<th>Жанр</th>
				<th>Год издания</th>
				<th>Цена</th>
			</tr>
			<tr>
				<td><xsl:value-of select="title"/></td>
				<td><xsl:value-of select="author"/></td>
				<td><xsl:value-of select="genre"/></td>
				<td><xsl:value-of select="year"/></td>
				<td><xsl:value-of select="price"/></td>
			</tr>
		</table>
	</xsl:template>
</xsl:stylesheet>