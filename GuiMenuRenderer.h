#ifndef GUIMENURENDERER_H
#define GUIMENURENDERER_H


class GuiMenuRenderer
{
    public:
        GuiMenuRenderer();
        drawMenu(Pager* _pager);
        virtual ~GuiMenuRenderer();
    protected:
    private:
        ofImage frame;
};

#endif // GUIMENURENDERER_H
