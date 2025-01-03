switch(msg->message)
    {
        case WM_KEYDOWN:
            down = 1;
            // Processar tecla pressionada...
            switch (msg->wParam)
            {
                case VK_SHIFT:
                    nk_input_key(ctx, NK_KEY_SHIFT, down);
                    break;
                case VK_DELETE:
                    nk_input_key(ctx, NK_KEY_DEL, down);
                    break;
                case VK_RETURN:
                    nk_input_key(ctx, NK_KEY_ENTER, down);
                    break;
                case VK_TAB:
                    nk_input_key(ctx, NK_KEY_TAB, down);
                    break;
                case VK_BACK:
                    nk_input_key(ctx, NK_KEY_BACKSPACE, down);
                    break;
                case VK_HOME:
                    nk_input_key(ctx, NK_KEY_TEXT_START, down);
                    nk_input_key(ctx, NK_KEY_SCROLL_START, down);
                    break;
                case VK_END:
                    nk_input_key(ctx, NK_KEY_TEXT_END, down);
                    nk_input_key(ctx, NK_KEY_SCROLL_END, down);
                    break;
                case VK_PRIOR:
                    nk_input_key(ctx, NK_KEY_SCROLL_DOWN, down);
                    break;
                case VK_NEXT:
                    nk_input_key(ctx, NK_KEY_SCROLL_UP, down);
                    break;
                case 'Z':
                    if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
                        nk_input_key(ctx, NK_KEY_TEXT_UNDO, down);
                    break;
                case 'R':
                    if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
                        nk_input_key(ctx, NK_KEY_TEXT_REDO, down);
                    break;
                case 'C':
                    if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
                        nk_input_key(ctx, NK_KEY_COPY, down);
                    break;
                case 'V':
                    if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
                        nk_input_key(ctx, NK_KEY_PASTE, down);
                    break;
                case 'X':
                    if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
                        nk_input_key(ctx, NK_KEY_CUT, down);
                    break;
                case 'B':
                    if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
                        nk_input_key(ctx, NK_KEY_TEXT_LINE_START, down);
                    break;
                case 'E':
                    if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
                        nk_input_key(ctx, NK_KEY_TEXT_LINE_END, down);
                    break;
                case VK_UP:
                    nk_input_key(ctx, NK_KEY_UP, down);
                    break;
                case VK_DOWN:
                    nk_input_key(ctx, NK_KEY_DOWN, down);
                    break;
                case VK_LEFT:
                    if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
                        nk_input_key(ctx, NK_KEY_TEXT_WORD_LEFT, down);
                    else
                        nk_input_key(ctx, NK_KEY_LEFT, down);
                    break;
                case VK_RIGHT:
                    if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
                        nk_input_key(ctx, NK_KEY_TEXT_WORD_RIGHT, down);
                    else
                        nk_input_key(ctx, NK_KEY_RIGHT, down);
                    break;
            }
            return 1;
        case WM_KEYUP:
            down = 0;
            // Processar tecla solta...
            return 1;
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
            // Processar eventos do mouse...
            return 1;
        case WM_MOUSEMOVE:
            // Processar movimento do mouse...
            return 1;
        case WM_MOUSEWHEEL:
            // Processar roldana do mouse...
            return 1;
        case WM_CHAR:
            // Processar entrada de texto...
            glyph[0] = (char)msg->wParam;
            nk_input_glyph(ctx, glyph);
            return 1;
    }


/* SDL COMMANDS */

    if (evt->type == SDL_KEYUP || evt->type == SDL_KEYDOWN) 
    {
        /* key events */
        down = evt->type == SDL_KEYDOWN;

        const Uint8* state = SDL_GetKeyboardState(0);

        SDL_Keycode sym = evt->key.keysym.sym;

        if(sym == SDLK_RSHIFT || sym == SDLK_LSHIFT)
            nk_input_key(ctx, NK_KEY_SHIFT, down);
        else if (sym == SDLK_DELETE)
            nk_input_key(ctx, NK_KEY_DEL, down);
        else if (sym == SDLK_RETURN)
            nk_input_key(ctx, NK_KEY_ENTER, down);
        else if (sym == SDLK_TAB)
            nk_input_key(ctx, NK_KEY_TAB, down);
        else if (sym == SDLK_BACKSPACE)
            nk_input_key(ctx, NK_KEY_BACKSPACE, down);
        else if (sym == SDLK_HOME) 
        {
            nk_input_key(ctx, NK_KEY_TEXT_START, down);
            nk_input_key(ctx, NK_KEY_SCROLL_START, down);
        } 
        else if (sym == SDLK_END) 
        {
            nk_input_key(ctx, NK_KEY_TEXT_END, down);
            nk_input_key(ctx, NK_KEY_SCROLL_END, down);
        } 
        else if (sym == SDLK_PAGEDOWN) 
        {
            nk_input_key(ctx, NK_KEY_SCROLL_DOWN, down);
        } 
        else if (sym == SDLK_PAGEUP) 
        {
            nk_input_key(ctx, NK_KEY_SCROLL_UP, down);
        } 
        else if (sym == SDLK_z)
            nk_input_key(ctx, NK_KEY_TEXT_UNDO, down && state[SDL_SCANCODE_LCTRL]);
        else if (sym == SDLK_r)
            nk_input_key(ctx, NK_KEY_TEXT_REDO, down && state[SDL_SCANCODE_LCTRL]);
        else if (sym == SDLK_c)
            nk_input_key(ctx, NK_KEY_COPY, down && state[SDL_SCANCODE_LCTRL]);
        else if (sym == SDLK_v)
            nk_input_key(ctx, NK_KEY_PASTE, down && state[SDL_SCANCODE_LCTRL]);
        else if (sym == SDLK_x)
            nk_input_key(ctx, NK_KEY_CUT, down && state[SDL_SCANCODE_LCTRL]);
        else if (sym == SDLK_b)
            nk_input_key(ctx, NK_KEY_TEXT_LINE_START, down && state[SDL_SCANCODE_LCTRL]);
        else if (sym == SDLK_e)
            nk_input_key(ctx, NK_KEY_TEXT_LINE_END, down && state[SDL_SCANCODE_LCTRL]);
        else if (sym == SDLK_UP)
            nk_input_key(ctx, NK_KEY_UP, down);
        else if (sym == SDLK_DOWN)
            nk_input_key(ctx, NK_KEY_DOWN, down);
        else if (sym == SDLK_LEFT) 
        {
            if (state[SDL_SCANCODE_LCTRL])
                nk_input_key(ctx, NK_KEY_TEXT_WORD_LEFT, down);
            else nk_input_key(ctx, NK_KEY_LEFT, down);
        } 
        else if (sym == SDLK_RIGHT) 
        {
            if (state[SDL_SCANCODE_LCTRL])
                nk_input_key(ctx, NK_KEY_TEXT_WORD_RIGHT, down);
            else nk_input_key(ctx, NK_KEY_RIGHT, down);
        } 
        else return 0;
        return 1;
    } 
    else if (evt->type == SDL_MOUSEBUTTONDOWN || evt->type == SDL_MOUSEBUTTONUP) 
    {
        /* mouse button */
        down = evt->type == SDL_MOUSEBUTTONDOWN;
        const int x = evt->button.x, y = evt->button.y;
        if (evt->button.button == SDL_BUTTON_LEFT) 
        {
            if (evt->button.clicks > 1)
                nk_input_button(ctx, NK_BUTTON_DOUBLE, x, y, down);
            nk_input_button(ctx, NK_BUTTON_LEFT, x, y, down);
        } 
        else if (evt->button.button == SDL_BUTTON_MIDDLE)
            nk_input_button(ctx, NK_BUTTON_MIDDLE, x, y, down);
        else if (evt->button.button == SDL_BUTTON_RIGHT)
            nk_input_button(ctx, NK_BUTTON_RIGHT, x, y, down);
        return 1;
    } 
    else if (evt->type == SDL_MOUSEMOTION) 
    {
        /* mouse motion */
        if (ctx->input.mouse.grabbed) 
        {
            x = (int)ctx->input.mouse.prev.x, y = (int)ctx->input.mouse.prev.y;
            nk_input_motion(ctx, x + evt->motion.xrel, y + evt->motion.yrel);
        } 
        else nk_input_motion(ctx, evt->motion.x, evt->motion.y);
        return 1;
    } 
    else if (evt->type == SDL_TEXTINPUT) 
    {
        /* text input */
        nk_glyph glyph;
        memcpy(glyph, evt->text.text, NK_UTF_SIZE);
        nk_input_glyph(ctx, glyph);
        return 1;
    } 
    else if (evt->type == SDL_MOUSEWHEEL) 
    {
        /* mouse wheel */
        nk_input_scroll(ctx,nk_vec2((float)evt->wheel.x,(float)evt->wheel.y));
        return 1;
    }