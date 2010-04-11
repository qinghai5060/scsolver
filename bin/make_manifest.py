#!/usr/bin/env python

import xml.dom.minidom, sys

def error (msg, abortAfter=True):
    sys.stderr.write(msg + "\n")
    if abortAfter:
        sys.exit(1)

def printNode (node, level=0):
    outstr = ''
    if level == 0:
        outstr += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    indent = "    "*level
    childCount = len(node.childNodes)
    nodeName = node.localName
    if node.prefix:
        nodeName = node.prefix + ":" + nodeName
    outstr += indent + '<'
    outstr += nodeName

    for key in node.attributes.keys():
        attr = node.attributes[key]
        if attr.prefix == None:
            outstr += " %s:%s=\"%s\""%(attr.prefix, attr.localName, attr.value)
        else:
            outstr += " %s:%s=\"%s\""%(attr.prefix, attr.localName, attr.value)

    if childCount == 0:
        outstr += "/>\n"
    else:
        outstr += ">\n"
    
        for child in node.childNodes:
            outstr += printNode(child, level+1)
    
        outstr += indent + "</%s>\n"%nodeName

    return outstr


def insertFileEntry (doc, parentNode, mediaType, fullPath):
    elem = doc.createElementNS(None, "manifest:file-entry")
    elem.setAttributeNS(None, "manifest:media-type", mediaType)
    elem.setAttributeNS(None, "manifest:full-path", fullPath)
    parentNode.appendChild(elem)


def main (args):
    if len(args) < 3:
        # We need at least the manifest.xml path (1st arg) and shared lib name (2nd arg).
        error("need at least two arguments")

    outpath = args[1]
    libname = args[2]
    platform = ''
    if len(args) >= 4:
        platform = args[3]

    impl = xml.dom.minidom.getDOMImplementation()
    doc = impl.createDocument(None, "manifest", None)
    root = doc.documentElement
    root.prefix = "manifest"
    root.setAttributeNS(None, "xmlns:manifest", "urn:oasis:names:tc:opendocument:xmlns:manifest:1.0")
    insertFileEntry(doc, root, "text/xml", "description.xml")

    # entry for the uno shared library.
    mediaType = "application/vnd.sun.star.uno-component;type=native"
    if len(platform) > 0:
        mediaType += ";platform=%s"%platform
    insertFileEntry(doc, root, mediaType, libname)

    insertFileEntry(doc, root, "application/vnd.sun.star.uno-typelibrary;type=RDB", "scsolver.uno.rdb")
    insertFileEntry(doc, root, "application/vnd.sun.star.configuration-data", "Addons.xcu")
    insertFileEntry(doc, root, "application/vnd.sun.star.configuration-data", "ProtocolHandler.xcu")
    insertFileEntry(doc, root, "application/vnd.sun.star.package-bundle-description;locale=en-US", "translation/en-US/readme.txt")
    insertFileEntry(doc, root, "application/vnd.sun.star.help", "help")
    
    outstr = printNode(root)
    file = open(outpath, 'w')
    file.write(outstr)
    file.close()

if __name__ == '__main__':
    main(sys.argv)
