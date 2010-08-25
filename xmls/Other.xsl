<xsl:stylesheet version = '1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>

<xsl:template match="/">
	<html>
	<body>
<xsl:apply-templates/>
	</body>
	</html>
</xsl:template>

<xsl:template match="Directory">
	<xsl:choose>
		<xsl:if test="@path">
			<b><font color="#0000ff"><xsl:value-of select="@path" /></font></b><br/>
		</xsl:if>
		<xsl:otherwise>
			<li>
			<b><font color="#0000ff"><xsl:value-of select="@name" /></font></b> <small> <i> (
			<xsl:value-of select="count(child::*[@name])"/> items) </i> </small>
			</li>
		</xsl:otherwise>
	</xsl:choose>
	<ul>
<xsl:apply-templates/>
	</ul>
</xsl:template>

<xsl:template match="File">
	<li>
	<i><xsl:value-of select="@name" /></i> <small> <i> (
	<xsl:value-of select="@size" /> bytes) </i></small>
	<xsl:if test="@type">
		__________ <font size="-2"><xsl:value-of select="@type" /></font>
	</xsl:if>
	</li>
</xsl:template>

<xsl:template match="Link">
	<li>
	@<i><font color="#00EEEE"><xsl:value-of select="@name" /></font></i>
	</li>
</xsl:template>

<xsl:template match="Socket">
	<li>
	&amp;<i><font color="EEEE00"><xsl:value-of select="@name" /></font></i>
	</li>
</xsl:template>

<xsl:template match="BlockDevice">
	<li>
	B<i><font color="B08060"><xsl:value-of select="@name" /></font></i>
	</li>
</xsl:template>

<xsl:template match="CharacterDevice">
	<li>
	C<i><font color="B08060"><xsl:value-of select="@name" /></font></i>
	</li>
</xsl:template>

<xsl:template match="Fifo">
	<li>
	<i><xsl:value-of select="@name" />|</i>
	</li>
</xsl:template>

</xsl:stylesheet>

