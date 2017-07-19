///*
//** Haaf's Game Engine 1.7
//** Copyright (C) 2003-2007, Relish Games
//** hge.relishgames.com
//**
//** hgeGUI default controls implementation
//*/
//
//
//#include "hgeguictrls.h"
//#include <string.h>
//#include <stdio.h>
//#include <stdlib.h>
//
//
///*
//** hgeGUIText
//*/
//
//hgeGUIText::hgeGUIText(int _id, float x, float y, float w, float h, hgeFont *fnt)
//{
//    id = _id;
//    bStatic = true;
//    bVisible = true;
//    bEnabled = true;
//    rect.Set(x, y, x + w, y + h);
//
//    font = fnt;
//    tx = x;
//    ty = y + (h - fnt->GetHeight()) / 2.0f;
//
//    text[0] = 0;
//}
//
//void hgeGUIText::SetMode(int _align)
//{
//    align = _align;
//    if (align == HGETEXT_RIGHT) tx = rect.x2;
//    else if (align == HGETEXT_CENTER) tx = (rect.x1 + rect.x2) / 2.0f;
//    else tx = rect.x1;
//}
//
//void hgeGUIText::SetText(const char *_text)
//{
//    strcpy(text, _text);
//}
//
//void hgeGUIText::printf(const char *format, ...)
//{
//    vsprintf(text, format, (char *)&format + sizeof(format));
//}
//
//void hgeGUIText::Render()
//{
//    font->SetColor(color);
//    font->Render(tx, ty, align, text);
//}
//
///*
//** hgeGUIButton
//*/
//
//hgeGUIButton::hgeGUIButton(int _id, float x, float y, float w, float h, HTEXTURE tex, float tx, float ty)
//{
//    id = _id;
//    bStatic = false;
//    bVisible = true;
//    bEnabled = true;
//    rect.Set(x, y, x + w, y + h);
//
//    bPressed = false;
//    bTrigger = false;
//
//    sprUp = new hgeSprite(tex, tx, ty, w, h);
//    sprDown = new hgeSprite(tex, tx + w, ty, w, h);
//}
//
//hgeGUIButton::~hgeGUIButton()
//{
//    if (sprUp) delete sprUp;
//    if (sprDown) delete sprDown;
//}
//
//void hgeGUIButton::Render()
//{
//    if (bPressed) sprDown->RenderTexture(rect.x1, rect.y1, 0xFFFFFFFF);
//    else sprUp->RenderTexture(rect.x1, rect.y1, 0xFFFFFFFF);
//}
//
//bool hgeGUIButton::MouseLButton(bool bDown)
//{
//    if (bDown)
//    {
//        bOldState = bPressed; bPressed = true;
//        return false;
//    }
//    else
//    {
//        if (bTrigger) bPressed = !bOldState;
//        else bPressed = false;
//        return true;
//    }
//}
//
///*
//** hgeGUISlider
//*/
//
//hgeGUISlider::hgeGUISlider(int _id, float x, float y, float w, float h, HTEXTURE tex, float tx, float ty, float sw, float sh, bool vertical)
//{
//    id = _id;
//    bStatic = false;
//    bVisible = true;
//    bEnabled = true;
//    bPressed = false;
//    bVertical = vertical;
//    rect.Set(x, y, x + w, y + h);
//
//    mode = HGESLIDER_BAR;
//    fMin = 0; fMax = 100; fVal = 50;
//    sl_w = sw; sl_h = sh;
//
//    sprSlider = new hgeSprite(tex, tx, ty, sw, sh);
//}
//
//hgeGUISlider::~hgeGUISlider()
//{
//    if (sprSlider) delete sprSlider;
//}
//
//void hgeGUISlider::SetValue(float _fVal)
//{
//    if (_fVal<fMin) fVal = fMin;
//    else if (_fVal>fMax) fVal = fMax;
//    else fVal = _fVal;
//}
//
//void hgeGUISlider::Render()
//{
//    float xx, yy;
//    float x1, y1, x2, y2;
//
//    xx = rect.x1 + (rect.x2 - rect.x1)*(fVal - fMin) / (fMax - fMin);
//    yy = rect.y1 + (rect.y2 - rect.y1)*(fVal - fMin) / (fMax - fMin);
//
//    if (bVertical)
//        switch (mode)
//        {
//            case HGESLIDER_BAR: x1 = rect.x1; y1 = rect.y1; x2 = rect.x2; y2 = yy; break;
//            case HGESLIDER_BARRELATIVE: x1 = rect.x1; y1 = (rect.y1 + rect.y2) / 2; x2 = rect.x2; y2 = yy; break;
//            case HGESLIDER_SLIDER: x1 = (rect.x1 + rect.x2 - sl_w) / 2; y1 = yy - sl_h / 2; x2 = (rect.x1 + rect.x2 + sl_w) / 2; y2 = yy + sl_h / 2; break;
//        }
//    else
//        switch (mode)
//        {
//            case HGESLIDER_BAR: x1 = rect.x1; y1 = rect.y1; x2 = xx; y2 = rect.y2; break;
//            case HGESLIDER_BARRELATIVE: x1 = (rect.x1 + rect.x2) / 2; y1 = rect.y1; x2 = xx; y2 = rect.y2; break;
//            case HGESLIDER_SLIDER: x1 = xx - sl_w / 2; y1 = (rect.y1 + rect.y2 - sl_h) / 2; x2 = xx + sl_w / 2; y2 = (rect.y1 + rect.y2 + sl_h) / 2; break;
//        }
//
//    sprSlider->RenderStretch(x1, y1, x2, y2);
//}
//
//bool hgeGUISlider::MouseLButton(bool bDown)
//{
//    bPressed = bDown;
//    return false;
//}
//
//bool hgeGUISlider::MouseMove(float x, float y)
//{
//    if (bPressed)
//    {
//        if (bVertical)
//        {
//            if (y > rect.y2 - rect.y1) y = rect.y2 - rect.y1;
//            if (y < 0) y = 0;
//            fVal = fMin + (fMax - fMin)*y / (rect.y2 - rect.y1);
//        }
//        else
//        {
//            if (x > rect.x2 - rect.x1) x = rect.x2 - rect.x1;
//            if (x < 0) x = 0;
//            fVal = fMin + (fMax - fMin)*x / (rect.x2 - rect.x1);
//        }
//        return true;
//    }
//
//    return false;
//}
//
//
///*
//** hgeGUIListbox
//*/
//
//hgeGUIListbox::hgeGUIListbox(int _id, float x, float y, float w, float h, hgeFont *fnt, DWORD tColor, DWORD thColor, DWORD hColor)
//{
//    id = _id;
//    bStatic = false;
//    bVisible = true;
//    bEnabled = true;
//    rect.Set(x, y, x + w, y + h);
//    font = fnt;
//    sprHighlight = new hgeSprite(0, 0, 0, w, fnt->GetHeight());
//    sprHighlight->SetColor(hColor);
//    textColor = tColor;
//    texthilColor = thColor;
//    pItems = 0;
//    nItems = 0;
//
//    nSelectedItem = 0;
//    nTopItem = 0;
//    mx = 0; my = 0;
//}
//
//hgeGUIListbox::~hgeGUIListbox()
//{
//    Clear();
//    if (sprHighlight) delete sprHighlight;
//}
//
//
//int hgeGUIListbox::AddItem(char *item)
//{
//    hgeGUIListboxItem *pItem = pItems, *pPrev = 0, *pNew;
//
//    pNew = new hgeGUIListboxItem;
//    memcpy(pNew->text, item, min(sizeof(pNew->text), strlen(item) + 1));
//    pNew->text[sizeof(pNew->text) - 1] = '\0';
//    pNew->next = 0;
//
//    while (pItem) { pPrev = pItem;	pItem = pItem->next; }
//
//    if (pPrev) pPrev->next = pNew;
//    else pItems = pNew;
//    nItems++;
//
//    return nItems - 1;
//}
//
//void hgeGUIListbox::DeleteItem(int n)
//{
//    int i;
//    hgeGUIListboxItem *pItem = pItems, *pPrev = 0;
//
//    if (n < 0 || n >= GetNumItems()) return;
//
//    for (i = 0; i < n; i++) { pPrev = pItem;	pItem = pItem->next; }
//
//    if (pPrev) pPrev->next = pItem->next;
//    else pItems = pItem->next;
//
//    delete pItem;
//    nItems--;
//}
//
//char *hgeGUIListbox::GetItemText(int n)
//{
//    int i;
//    hgeGUIListboxItem *pItem = pItems;
//
//    if (n < 0 || n >= GetNumItems()) return 0;
//
//    for (i = 0; i < n; i++) pItem = pItem->next;
//
//    return pItem->text;
//}
//
//void hgeGUIListbox::Clear()
//{
//    hgeGUIListboxItem *pItem = pItems, *pNext;
//
//    while (pItem)
//    {
//        pNext = pItem->next;
//        delete pItem;
//        pItem = pNext;
//    }
//
//    pItems = 0;
//    nItems = 0;
//}
//
//void hgeGUIListbox::Render()
//{
//    int i;
//    hgeGUIListboxItem *pItem = pItems;
//
//    for (i = 0; i < nTopItem; i++) pItem = pItem->next;
//    for (i = 0; i < GetNumRows(); i++)
//    {
//        if (i >= nItems) return;
//
//        if (nTopItem + i == nSelectedItem)
//        {
//            sprHighlight->RenderTexture(rect.x1, rect.y1 + i*font->GetHeight(), 0xFFFFFFFF);
//            font->SetColor(texthilColor);
//        }
//        else
//            font->SetColor(textColor);
//
//        font->Render(rect.x1 + 3, rect.y1 + i*font->GetHeight(), HGETEXT_LEFT, pItem->text);
//        pItem = pItem->next;
//    }
//}
//
//bool hgeGUIListbox::MouseLButton(bool bDown)
//{
//    int nItem;
//
//    if (bDown)
//    {
//        nItem = nTopItem + int(my) / int(font->GetHeight());
//        if (nItem < nItems)
//        {
//            nSelectedItem = nItem;
//            return true;
//        }
//    }
//    return false;
//}
//
//
//bool hgeGUIListbox::MouseWheel(int nNotches)
//{
//    nTopItem -= nNotches;
//    if (nTopItem < 0) nTopItem = 0;
//    if (nTopItem > GetNumItems() - GetNumRows()) nTopItem = GetNumItems() - GetNumRows();
//
//    return true;
//}
//
//bool hgeGUIListbox::KeyClick(int key, int chr)
//{
//    switch (key)
//    {
//        case HGEK_DOWN:
//            if (nSelectedItem < nItems - 1)
//            {
//                nSelectedItem++;
//                if (nSelectedItem > nTopItem + GetNumRows() - 1) nTopItem = nSelectedItem - GetNumRows() + 1;
//                return true;
//            }
//            break;
//
//        case HGEK_UP:
//            if (nSelectedItem > 0)
//            {
//                nSelectedItem--;
//                if (nSelectedItem < nTopItem) nTopItem = nSelectedItem;
//                return true;
//            }
//            break;
//    }
//    return false;
//}
//
////////////////////////////////////////////////////////////////////////
//// hgeGUILabel
////////////////////////////////////////////////////////////////////////
//
//hgeGUILabel::hgeGUILabel(int _id, float x, float y, hgeFont *fnt, char* text, DWORD color, int align)
//{
//    bStatic = true;
//    bVisible = true;
//    bEnabled = true;
//    id = _id;
//
//    m_Font = fnt;
//    m_Pos.x = x;
//    m_Pos.y = y;
//    m_Color = color;
//
//    m_Align = align;
//
//    SetText(text);
//}
//
//
//
//void hgeGUILabel::SetVisible(bool yesno)
//{
//    bVisible = yesno;
//}
//
//
//
//void hgeGUILabel::SetText(int v)
//{
//    char str[16];
//    _itoa(v, str, 10);
//    SetText(str);
//}
//
//
//
//void hgeGUILabel::SetText(char* text)
//{
//    strncpy(m_Text, text, 256);
//    m_Text[255] = 0;
//
//    char* p = strchr(m_Text, '\n');
//    int lines = 1;
//
//    float w = m_Font->GetStringWidth(m_Text);
//
//    while (p++)               // skip '\n' character
//    {
//        float tw = m_Font->GetStringWidth(p);
//        if (tw > w)   w = tw;
//
//        p = strchr(p, '\n');
//        lines++;
//    }
//
//    float h = m_Font->GetHeight() * m_Font->GetScale() * lines;
//
//    switch (m_Align)
//    {
//        case HGETEXT_LEFT:
//            rect.Set(m_Pos.x, m_Pos.y, m_Pos.x + w, m_Pos.y + h);
//            break;
//
//        case HGETEXT_CENTER:
//            rect.Set(m_Pos.x - w / 2, m_Pos.y, m_Pos.x + w / 2, m_Pos.y + h);
//            break;
//
//        case HGETEXT_RIGHT:
//            rect.Set(m_Pos.x - w, m_Pos.y, m_Pos.x, m_Pos.y + h);
//            break;
//    }
//}
//
//
//
//void hgeGUILabel::SetAlign(int align)
//{
//    m_Align = align;
//
//    float w = rect.x2 - rect.x1;
//    float h = rect.y2 - rect.y1;
//
//    switch (m_Align)
//    {
//        case HGETEXT_LEFT:
//            rect.Set(m_Pos.x, m_Pos.y, m_Pos.x + w, m_Pos.y + h);
//            break;
//
//        case HGETEXT_CENTER:
//            rect.Set(m_Pos.x - w / 2, m_Pos.y, m_Pos.x + w / 2, m_Pos.y + h);
//            break;
//
//        case HGETEXT_RIGHT:
//            rect.Set(m_Pos.x - w, m_Pos.y, m_Pos.x, m_Pos.y + h);
//            break;
//    }
//}
//
//
//
//void hgeGUILabel::SetPos(float x, float y)
//{
//    m_Pos.x = x;
//    m_Pos.y = y;
//
//    SetAlign(m_Align);
//}
//
//
//
//void hgeGUILabel::SetColor(DWORD col)
//{
//    m_Color = col;
//}
//
//
//
//void hgeGUILabel::Render()
//{
//    m_Font->SetColor(m_Color);
//    m_Font->Render(m_Pos.x, m_Pos.y, m_Align, (const char*)m_Text);
//}
//
//
//
///////////////////////////////////////////////////////////////////////
//// hgeGUIMemo
////////////////////////////////////////////////////////////////////////
//
//hgeGUIMemo::hgeGUIMemo(int _id, float x, float y, float w, float h, hgeFont *fnt, char* text, DWORD color)
//{
//    bStatic = true;
//    bVisible = true;
//    bEnabled = true;
//    id = _id;
//
//    m_Font = fnt;
//    m_Pos.x = x;
//    m_Pos.y = y;
//    m_Color = color;
//
//    m_Width = w;
//    m_Height = h;
//
//    rect.Set(x, y, x + w, y + h);
//
//    SetText(text);
//}
//
//
//
//void hgeGUIMemo::SetVisible(bool yesno)
//{
//    bVisible = yesno;
//}
//
//
//
//void hgeGUIMemo::SetText(char* text)
//{
//    int lines = 1;
//    char *d = m_Text;
//    char *t = m_Text;
//    memset(m_Text, 0, sizeof(m_Text));
//
//    for (char *p = text; p < text + 512; p++, d++)
//    {
//        if (!*p)
//            break;
//
//        *d = *p;
//
//        if (*d == '\n')
//        {
//            t = d + 1;
//            p = text + int(d - m_Text);
//        }
//
//        float tw = m_Font->GetStringWidth(t);
//
//        if (tw > m_Width)
//        {
//            d = strrchr(t, ' ') + 1;
//            p = text + int(d - m_Text) - 1;
//
//            *d = '\n';
//            t = d;
//
//            lines++;
//
//            float h = m_Font->GetHeight() * m_Font->GetScale() * lines;
//            if (h > m_Height)
//                break;
//        }
//    }
//}
//
//
//
//void hgeGUIMemo::SetPos(float x, float y)
//{
//    m_Pos.x = x;
//    m_Pos.y = y;
//}
//
//
//
//void hgeGUIMemo::SetColor(DWORD col)
//{
//    m_Color = col;
//}
//
//
//
//void hgeGUIMemo::Render()
//{
//    m_Font->SetColor(m_Color);
//    m_Font->Render(m_Pos.x, m_Pos.y, HGETEXT_LEFT, m_Text);
//}
//
//
////////////////////////////////////////////////////////////////////////
//// hgeGUIImage
////////////////////////////////////////////////////////////////////////
//
//
//hgeGUIImage::hgeGUIImage(int _id, float x, float y, char* file)
//{
//    bStatic = true;
//    bVisible = true;
//    bEnabled = true;
//    id = _id;
//
//    HTEXTURE tex = G_HGE->Texture_LoadFromFile(file);
//
//    m_Width = (float)G_HGE->Texture_GetWidth(tex);
//    m_Height = (float)G_HGE->Texture_GetHeight(tex);
//
//    rect.Set(x, y, x + m_Width, y + m_Height);
//
//    for (int i = 0; i < 4; i++)
//    {
//        m_Quad.v[i].z = 0.5f;
//        m_Quad.v[i].col = 0xFFFFFFFF;
//    }
//
//    m_Quad.blend = BLEND_ALPHABLEND | BLEND_COLORMUL | BLEND_ZWRITE;
//    m_Quad.tex = tex;
//
//    m_Quad.v[0].x = x;               m_Quad.v[0].y = y;
//    m_Quad.v[1].x = x + m_Width;   m_Quad.v[1].y = y;
//    m_Quad.v[2].x = x + m_Width;   m_Quad.v[2].y = y + m_Height;
//    m_Quad.v[3].x = x;               m_Quad.v[3].y = y + m_Height;
//
//    m_Quad.v[0].tx = 0;               m_Quad.v[0].ty = 0;
//    m_Quad.v[1].tx = 1;               m_Quad.v[1].ty = 0;
//    m_Quad.v[2].tx = 1;               m_Quad.v[2].ty = 1;
//    m_Quad.v[3].tx = 0;               m_Quad.v[3].ty = 1;
//}
//
//
//
//hgeGUIImage::~hgeGUIImage()
//{
//    G_HGE->Texture_Free(m_Quad.tex);
//}
//
//
//
//void hgeGUIImage::SetPos(float x, float y)
//{
//    m_Quad.v[0].x = x;               m_Quad.v[0].y = y;
//    m_Quad.v[1].x = x + m_Width;   m_Quad.v[1].y = y;
//    m_Quad.v[2].x = x + m_Width;   m_Quad.v[2].y = y + m_Height;
//    m_Quad.v[3].x = x;               m_Quad.v[3].y = y + m_Height;
//}
//
//
//void hgeGUIImage::Render()
//{
//    G_HGE->Gfx_RenderQuad(&m_Quad);
//}
//
//
////////////////////////////////////////////////////////////////////////
//// hgeGUIEditBox
////////////////////////////////////////////////////////////////////////
//
//
//hgeGUIEditBox::hgeGUIEditBox(int _id, float x, float y, float w, hgeFont* fnt, char* str)
//{
//    bStatic = false;
//    bVisible = true;
//    bEnabled = true;
//    id = _id;
//
//    m_Font = fnt;
//    m_Focus = false;
//
//    m_Time = 0;
//    m_Blink = 0;
//    m_blink_col = 0xFFFFFFFF;
//
//    strncpy(m_Text, str, 64);
//    m_Pos = (int)strlen(m_Text);
//
//    rect.Set(x - 2, y - 2, x + w + 4, y + fnt->GetHeight()*fnt->GetScale() + 4);
//}
//
//
//
//bool hgeGUIEditBox::KeyClick(int key, int chr)
//{
//    int i, len = (int)strlen(m_Text);
//
//    switch (key)
//    {
//        case HGEK_ENTER:
//            gui->SetFocus(0);
//            break;
//
//        case HGEK_LEFT:
//            if (m_Pos > 0) m_Pos--;
//            break;
//
//        case HGEK_RIGHT:
//            if (m_Pos < (int)strlen(m_Text)) m_Pos++;
//            break;
//
//        case HGEK_DELETE:
//            for (i = m_Pos; i < len; i++)   m_Text[i] = m_Text[i + 1];
//            break;
//
//        case HGEK_BACKSPACE:
//            if (m_Pos == 0)
//                break;
//
//            for (i = m_Pos; i < len + 1; i++) m_Text[i - 1] = m_Text[i];
//            m_Pos--;
//            break;
//
//        default:
//        {
//            if (chr == 0)
//                break;
//
//            char tmp[64];
//            strcpy(tmp, m_Text);
//
//            for (i = len; i > m_Pos && i>0; i--) m_Text[i] = m_Text[i - 1];
//            m_Text[m_Pos] = chr;
//            m_Text[len + 1] = 0;
//            m_Pos++;
//
//            if (m_Font->GetStringWidth(m_Text) > rect.x2 - rect.x1)
//            {
//                strcpy(m_Text, tmp);
//                m_Pos--;
//            }
//        }
//        break;
//    };
//
//    // keep caret from vanishing
//    m_Blink = true;
//    m_Time = 0;
//
//    return false;
//}
//
//
//
//bool hgeGUIEditBox::MouseLButton(bool bDown)
//{
//    if (bDown) gui->SetFocus(id);
//
//    return false;
//}
//
//
//
//void hgeGUIEditBox::Focus(bool bFocused)
//{
//    m_Focus = bFocused;
//}
//
//
//
//void hgeGUIEditBox::Update(float dt)
//{
//    if (m_Focus)
//    {
//        m_Time += dt;
//
//        if (m_Time > 0.3)
//        {
//            m_Time = 0;
//            m_Blink = !m_Blink;
//        }
//    }
//}
//
//
//
//void hgeGUIEditBox::Render()
//{
//    G_HGE->Gfx_RenderLine(rect.x1, rect.y1, rect.x2, rect.y1);
//    G_HGE->Gfx_RenderLine(rect.x2, rect.y1, rect.x2, rect.y2);
//    G_HGE->Gfx_RenderLine(rect.x2, rect.y2, rect.x1, rect.y2);
//    G_HGE->Gfx_RenderLine(rect.x1, rect.y2, rect.x1, rect.y1);
//
//    m_Font->SetColor(this->color/*0xFFC0C0C0*/);
//    m_Font->Render(rect.x1 + 2, rect.y1 + 2, HGETEXT_LEFT, m_Text);
//
//    if (m_Focus && m_Blink)
//    {
//        char tmp[64];
//        strncpy(tmp, m_Text, m_Pos);
//        tmp[m_Pos] = 0;
//
//        float p = m_Font->GetStringWidth(tmp);
//        G_HGE->Gfx_RenderLine(rect.x1 + 2 + p, rect.y1 + 2, rect.x1 + 2 + p, rect.y2 - 2, m_blink_col/*0xFFFFFFFF*/);
//    }
//}
//
//
//
////////////////////////////////////////////////////////////////////////
//// hgeGUIMenuItem
////////////////////////////////////////////////////////////////////////
//
//
//
//hgeGUIMenuItem::hgeGUIMenuItem(int _id, float x, float y, LPD3DXFONT fnt, char *text, HEFFECT snd)
//{
//    bStatic = false;
//    bVisible = true;
//    bEnabled = true;
//    id = _id;
//
//    m_Font = fnt;
//    m_Sound = snd;
//    m_Up = true;
//    m_IPol = 0;
//    m_APol = 0;
//    m_Focus = false;
//    m_Pos.x = x;
//    m_Pos.y = y;
//
//    m_NColor.SetHWColor(0xFFC0C030);
//    m_HColor.SetHWColor(0xFFFFFFFF);
//
//    SetText(text);
//}
//
//
//float hgeGUIMenuItem::GetStringWidth(char* text)
//{
//    TEXTMETRICA xTextmetrica;
//    m_Font->GetTextMetricsA(&xTextmetrica);
//    return (float)(xTextmetrica.tmAveCharWidth * strlen(text));
//}
//
//
//float hgeGUIMenuItem::GetStringHeight()
//{
//    TEXTMETRICA xTextmetrica;
//    m_Font->GetTextMetricsA(&xTextmetrica);
//    return (float)(xTextmetrica.tmHeight);
//}
//
//void hgeGUIMenuItem::SetText(char* text)
//{
//    strncpy(m_Text, text, 64);
//    m_Text[63] = 0;
//
//    char* p = strchr(m_Text, '\n');
//    int lines = 1;
//
//    float w = this->GetStringWidth(m_Text) + 10.0f;
//
//    while (p++)
//    {
//        float tw = this->GetStringWidth(p);
//        if (tw > w)   w = tw;
//
//        p = strchr(p, '\n');               // skip '\n' character
//        lines++;
//    }
//
//    //float h = m_Font->GetStringHeight() * m_Font->GetScale() * lines;
//    float h = this->GetStringHeight() * lines;
//
//    rect.Set(m_Pos.x - w / 2, m_Pos.y, m_Pos.x + w / 2, m_Pos.y + h);
//    //rect.Set(m_Pos.x, m_Pos.y, m_Pos.x + w, m_Pos.y + h);
//}
//
//void hgeGUIMenuItem::Render()
//{
//    tagRECT xRect = { (LONG)rect.x1, (LONG)rect.y1, (LONG)rect.x2, (LONG)rect.y2 };
//    m_Font->DrawTextA(nullptr, m_Text, -1, &xRect, HGETEXT_LEFT, hgeGUIObject::color);
//    //m_Font->SetColor(m_Color.GetHWColor());
//    //m_Font->Render(rect.x1, rect.y1, HGETEXT_LEFT, (const char*)m_Text);
//}
//
//void hgeGUIMenuItem::Update(float dt)
//{
//    if (m_APol < 1.0f)
//    {
//        m_APol += 5 * dt;
//        if (m_APol > 1)
//            m_APol = 1;
//
//        m_Color.a = (m_Up) ? m_APol : 1 - m_APol;
//    }
//
//    if (m_Focus)
//    {
//        m_IPol += 3 * dt;
//        if (m_IPol > 1)
//            m_IPol = 1;
//    }
//    else
//    {
//        m_IPol -= 3 * dt;
//        if (m_IPol < 0)
//            m_IPol = 0;
//    }
//
//    m_Color.r = (1.0f - m_IPol) * m_NColor.r + m_IPol * m_HColor.r;
//    m_Color.g = (1.0f - m_IPol) * m_NColor.g + m_IPol * m_HColor.g;
//    m_Color.b = (1.0f - m_IPol) * m_NColor.b + m_IPol * m_HColor.b;
//}
//
//
//void hgeGUIMenuItem::Enter()
//{
//    m_Up = true;
//    m_APol = 0;
//    m_IPol = 0;
//    m_Focus = false;
//}
//
//
//
//void hgeGUIMenuItem::Leave()
//{
//    m_Up = false;
//    m_APol = 0;
//}
//
//
//
//bool hgeGUIMenuItem::IsDone()
//{
//    return m_APol >= 1.0f;
//}
//
//
//
//void hgeGUIMenuItem::Focus(bool bFocused)
//{
//    m_Focus = bFocused;
//
//    if (m_Focus)
//        G_HGE->Effect_Play(m_Sound);
//}
//
//
//
//void hgeGUIMenuItem::MouseOver(bool bOver)
//{
//    if (bOver)
//        gui->SetFocus(id);
//}
//
//
//
//bool hgeGUIMenuItem::MouseLButton(bool bDown)
//{
//    if (bDown)
//    {
//        G_HGE->Effect_Play(m_Sound);
//        return false;
//    }
//
//    return true;
//}
//
//
//
//bool hgeGUIMenuItem::KeyClick(int key, int chr)
//{
//    if (key == HGEK_ENTER || key == HGEK_SPACE)
//    {
//        G_HGE->Effect_Play(m_Sound);
//        return true;
//    }
//
//    return false;
//}