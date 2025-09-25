Descrição
Componente de preview para ESP (visualização de elementos HUD) desenhado com ImGui/DirectX — projetado apenas como componente de interface para fins educacionais e de desenvolvimento. Este repositório demonstra como desenhar um preview local (caixas, linha para jogador, barra de vida, arma) dentro de um overlay ImGui, para ajustar estilos e configurações de UI sem aceder memória ou integrar funcionalidades de jogo reais.

Aviso legal / Ético
Este projeto e a amostra de código são para estudo, design de UI e integrações legítimas (testes em ambientes controlados, desenvolvimento de ferramentas autorizadas). Não use este código para trapacear em servidores online, burlar regras de jogo ou aceder sistemas sem permissão expressa. O autor e contribuidores recusam qualquer uso malicioso.

Recursos / Features

Componente de preview desenhado com ImDrawList do ImGui.

Mostra: caixa de canto (corner box), filled box (preenchimento leve), ESP line (linha até o jogador), barra de vida, nome do jogador, arma.

Parametrizável: escala, toggles para cada elemento (name/box/line/health/filled/weapon).


Repositório completo do projeto

Código completo do projeto (onde este componente pode ser integrado):
https://github.com/igr0t/AssaultClubExternalProject

Overlay credit:
Este README reconhece e dá crédito ao overlay base usado para a UI:
https://github.com/carlgwastaken/Overlay/tree/master

Requisitos (desenvolvimento)

C++17 (ou superior) — compilador (g++ / MSVC / clang)

ImGui (v1.XX) integrado ao projeto

Backend/renderer compatível (ex.: DirectX11 + ImGui_ImplDX11 + ImGui_ImplWin32) — o preview usa ImDrawList e funções ImGui



mkdir C:\path\to\repo\images


Copy-Item "C:\Users\igdahora\Downloads\imagePreview.png" "C:\path\to\repo\images\imagePreview.png"
