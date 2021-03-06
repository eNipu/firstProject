/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "CButton.h"
#include "../SRC/ByteStream.h"
#include "../SRC/ErrorCode.h"

CButton::CButton() {
    m_bIsInit = FALSE;
    m_usBtXCoord = -1;
    m_usBtYCoord = -1;
    m_usBtXSize = -1;
    m_usBtYSize = -1;
    m_usFontSize = d_FONT_16x30;
    m_ulForeGndColor = RGB(211, 211, 211);
    m_ulClickColor = RGB(190, 190, 190);
}

CButton::~CButton() {

}

/*
 * Init
 * 
 * Function:
 *          Init a button which has default font size and color.
 * 
 * @param
 *          usX: X coordinate, the range is from 0 to d_LCD_CANVAS_X_SIZE-1.
 *          usY: Y coordinate, the range is from 0 to d_LCD_CANVAS_Y_SIZE-1.
 *          usXSize: Width of the box.
 *          usYSize: Height of the box.
 * 
 * @return 
 *          OK - Init successfully.
 *          -1 - Init failed.
 */
USHORT CButton::Init(unsigned short usX, unsigned short usY, unsigned short usXSize, unsigned short usYSize) {
    DEBUG_ENTRY();

    if (usX < 0 || usY < 0 || usXSize < 3 || usYSize < 3 || usX > 320 || usY > 480)
        return DEBUG_RETCODE("Wrong Paramter!", -1);

    m_usBtXCoord = usX;
    m_usBtYCoord = usY;
    m_usBtXSize = usXSize;
    m_usBtYSize = usYSize;
    m_bIsInit = TRUE;

    return DEBUG_EXIT(OK);
}

/*
 * ButtonCaption
 * 
 * Function:
 *          Save the caption of button.
 * 
 * @param
 *          cBtCaption :The data to save.
 *          usDataLen  :Length of the data.
 * 
 * @return 
 *          OK - Save data successfully.
 *          -1 - Failed.
 */
USHORT CButton::ButtonCaption(CByteStream& cBtCaption, unsigned short usDataLen) {
    if (!m_bIsInit)
        return DEBUG_RETCODE("Don't open!", -1);

    if (usDataLen < 0)
        return DEBUG_RETCODE("Wrong Paratmer!", -1);

    m_cText.Write(cBtCaption.GetBuffer(), usDataLen);

    return DEBUG_EXIT(OK);
}

/*
 * SetFont
 * 
 * Function:
 *          Set the font size of the caption,it have default size 16x30.
 * 
 * @param
 *          usFont :The size of font.
 * 
 * @return 
 *          OK - Set font size successfully.
 *          -1 - Failed.
 */
USHORT CButton::SetFontSize(unsigned short usFont) {
    if (!m_bIsInit)
        return DEBUG_RETCODE("Don't open!", -1);

    if (usFont < 0 || usFont > 0x1428)
        return DEBUG_RETCODE("Wrong Paramter!", -1);

    m_usFontSize = usFont;
}

/*
 * ShowButton
 * 
 * Function:
 *          Show button.
 * 
 * @param
 *          None.
 * 
 * @return 
 *          OK - Set font size successfully.
 *          Other values - Please refer to ErrorCode.h.
 */
USHORT CButton::ShowButton() {
    DEBUG_ENTRY();
    if (!m_bIsInit)
        return DEBUG_RETCODE("Don't open!", -1);

    USHORT usRet = 0;
    USHORT usFontSizeH = m_usFontSize / 256;
    USHORT usFontSizeL = m_usFontSize % 256;
    USHORT usX = (m_usBtXSize / 2 - usFontSizeH / 2) + m_usBtXCoord;
    USHORT usY = (m_usBtYSize / 2 - usFontSizeL / 2) + m_usBtYCoord;

    CTOS_LCDForeGndColor(RGB(0, 0, 0));
    CTOS_LCDGSetBox(m_usBtXCoord, m_usBtYCoord, m_usBtXSize, m_usBtYSize, 1);
    CTOS_LCDForeGndColor(m_ulForeGndColor);
    CTOS_LCDGSetBox(m_usBtXCoord + 1, m_usBtYCoord + 1, m_usBtXSize - 2, m_usBtYSize - 2, 1);

    CTOS_LCDBackGndColor(m_ulForeGndColor);
    CTOS_LCDForeGndColor(RGB(0, 0, 0));
    usRet = CTOS_LCDGTextOut(usX, usY, (BYTE*) m_cText.PushZero(), m_usFontSize, d_FALSE);
    if (usRet != d_OK)
        return DEBUG_RETCODE("Fail to TextOut:%04X", usRet);

    return DEBUG_EXIT(OK);
}

/*
 * HideButton
 * 
 * Function:
 *          Hide button.
 * 
 * @param
 *          None.
 * 
 * @return 
 *          OK - Hide button successfully.
 *          -1 - Failed.
 */
USHORT CButton::HideButton() {
    DEBUG_ENTRY();
    if (!m_bIsInit)
        return DEBUG_RETCODE("Don't open!", -1);

    CTOS_LCDForeGndColor(RGB(255, 255, 255));
    CTOS_LCDGSetBox(m_usBtXCoord, m_usBtYCoord, m_usBtXCoord, m_usBtYSize, 1);

    return DEBUG_EXIT(OK);
}

/*
 * ClickButton
 * 
 * Function:
 *          It will change the color when call this function.
 * 
 * @param
 *          None.
 * 
 * @return 
 *          OK - Simulate the time when the button is clicked.
 *          Other values - Please refer to ErrorCode.h.
 */
USHORT CButton::ClickButton() {
    DEBUG_ENTRY();

    if (!m_bIsInit)
        return DEBUG_RETCODE("Don't open!", -1);

    USHORT usRet = 0;
    USHORT usFontSizeH = m_usFontSize / 256;
    USHORT usFontSizeL = m_usFontSize % 256;
    USHORT usX = (m_usBtXSize / 2 - usFontSizeH / 2) + m_usBtXCoord;
    USHORT usY = (m_usBtYSize / 2 - usFontSizeL / 2) + m_usBtYCoord;

    CTOS_LCDForeGndColor(m_ulClickColor);
    CTOS_LCDGSetBox(m_usBtXCoord + 1, m_usBtYCoord + 1, m_usBtXSize - 2, m_usBtYSize - 2, 1);

    CTOS_LCDBackGndColor(m_ulClickColor);
    CTOS_LCDForeGndColor(RGB(0, 0, 0));
    usRet = CTOS_LCDGTextOut(usX, usY, (BYTE*) m_cText.PushZero(), m_usFontSize, d_FALSE);
    if (usRet != d_OK)
        return DEBUG_RETCODE("Fail to TextOut:%04X", usRet);

    usleep(80000);
    CTOS_LCDForeGndColor(m_ulForeGndColor);
    CTOS_LCDGSetBox(m_usBtXCoord + 1, m_usBtYCoord + 1, m_usBtXSize - 2, m_usBtYSize - 2, 1);

    CTOS_LCDBackGndColor(m_ulForeGndColor);
    CTOS_LCDForeGndColor(RGB(0, 0, 0));
    usRet = CTOS_LCDGTextOut(usX, usY, (BYTE*) m_cText.PushZero(), m_usFontSize, d_FALSE);
    if (usRet != d_OK)
        return DEBUG_RETCODE("Fail to TextOut:%04X", usRet);

    return DEBUG_EXIT(OK);
}

USHORT CButton::ChangeForeGndColor(unsigned long ulColor) {
    if (!m_bIsInit)
        return DEBUG_RETCODE("Don't open!", -1);

    if (ulColor > RGB(255, 255, 255) || ulColor < RGB(0, 0, 0))
        return DEBUG_RETCODE("Wrong paramter!", -1);

    m_ulForeGndColor = ulColor;

    return DEBUG_EXIT(OK);
}
