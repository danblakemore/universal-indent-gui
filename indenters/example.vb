Dim docRoot As New ChilkatXml

Dim success As Long

docRoot.Tag = "myDoc"

'  To zip compress the content, set this flag to 1
Dim zipContent As Long
zipContent = 0
'  To 128-bit AES encrypt the content, set this flag to 1
Dim encryptContent As Long
encryptContent = 0
Dim encryptPassword As String
encryptPassword = ""

Dim pdfNode As ChilkatXml

Set pdfNode = docRoot.NewChild("pdf","")

'  Embed a PDF into XML
success = pdfNode.SetBinaryContentFromFile("sample.pdf",zipContent,encryptContent,encryptPassword)
If (success <> 1) Then
    MsgBox pdfNode.LastErrorText

    Exit Sub
End If

MsgBox pdfNode.LastErrorText

'  Display the entire XML document:
Text1.Text = Text1.Text & docRoot.GetXml() & vbCrLf

'  Get the Base64-encoded content and display it:
Text1.Text = Text1.Text & pdfNode.Content & vbCrLf

'  Extract the binary content from XML:
Dim unzipContent As Long
unzipContent = 0
Dim decryptContent As Long
decryptContent = 0
Dim decryptPassword As String
decryptPassword = ""
success = pdfNode.SaveBinaryContent("out.pdf",unzipContent,decryptContent,decryptPassword)
If (success <> 1) Then
    MsgBox pdfNode.LastErrorText

    Exit Sub
End If

MsgBox "Success!"