int extract_pic_info(const BYTE *pic, const uint32_t size, int &width, int &height)
{
    int ret = -1;
    width = 0;
    height = 0;
    size_t offset = 0;
    if (pic == NULL)
        return ret;
    if ((pic[0] == 'G') && (pic[1] == 'I') && (pic[2] == 'F') && (pic[3] == '8') && (pic[4] == '9' || '7') && (pic[5] == 'a'))
    {
        //gif
        offset = 6;
        width = MAKEUS(pic[offset + 1], pic[offset]);
        offset += 2;
        height = MAKEUS(pic[offset + 1], pic[offset]);
        ret = 0;
        SS_DEBUG((LM_DEBUG, "extract_pic_info: gif width(%d) height(%d)!\n", width, height));
    } else if ((pic[0] == 'B') && (pic[1] == 'M'))
    {
        //BMP
        offset = 18;
        width = MAKEUI(pic[offset + 3], pic[offset + 2], pic[offset + 1], pic[offset + 0]);
        offset += 4;
        height = MAKEUS(pic[offset + 1], pic[offset]);
        ret = 0;
        SS_DEBUG((LM_DEBUG, "extract_pic_info: BMP width(%d) height(%d)!\n", width, height));
    } else if (pic[0] == png_signature[0] && pic[1] == png_signature[1] && pic[2] == png_signature[2] 
            && pic[3] == png_signature [3] && pic[4] == png_signature[4] 
            && pic[5] == png_signature[5] && pic[6] == png_signature[6] && pic[7] == png_signature[7])
    {
        //PNG
        offset = 16;
        width = MAKEUI(pic[offset + 0], pic[offset + 1], pic[offset + 2], pic[offset + 3]);
        offset += 4;
        height = MAKEUI(pic[offset + 0], pic[offset + 1], pic[offset + 2], pic[offset + 3]);
        ret = 0;
        SS_DEBUG((LM_DEBUG, "extract_pic_info: PNG width(%d) height(%d)!\n", width, height));
    } else if (pic[0] == 255 && pic[1] == 216 && pic[size-2] == 255 && pic[size-1] == 217)
    {
        //JPEG
        bool finish = false;
        offset = 0;
        unsigned char id = 0;
        while(!finish && offset < size)
        {
            if (pic[offset++] != 0xff || offset >= size)
            {
                ret = -2;
                SS_DEBUG((LM_DEBUG, "extract_pic_info: JPEG format error!\n"));
                break;
            }
            id = pic[offset++];
            if (id >= M_APP0 && id <= M_APPF) // app data block
            {
                offset += MAKEUS(pic[offset], pic[offset + 1]);
                continue;
            }
            switch(id)
            {
                case M_SOI:
                    break;
                case M_COM:
                case M_DQT:
                case M_DHT:
                case M_DNL:
                case M_DRI:
                    offset += MAKEUS(pic[offset], pic[offset + 1]);
                    break;
                case M_SOF0:
                    offset += 3;
                    height = MAKEUS(pic[offset], pic[offset + 1]);
                    offset += 2;
                    width = MAKEUS(pic[offset], pic[offset + 1]);
                    finish = true;
                    ret = 0;
                    SS_DEBUG((LM_DEBUG, "extract_pic_info: JPEG width(%d) height(%d)!\n", width, height));
                    break;
                case M_SOS:
                case M_EOI:
                case M_DATA:
                    finish = true;
                    ret = -1;
                    SS_DEBUG((LM_DEBUG, "extract_pic_info: JPEG can't find SOF0!\n"));
                    break;
                default:
                    offset += MAKEUS(pic[offset], pic[offset + 1]);
                    break;
            }
        }
    }
    return ret;
}