# Redpak Devtool

Redpak devtool provides a graphical representation of containers/nodes inheritance within a binding/board.

## Navigation Bar

At the top of the UI a ![web socket connected](images/message_wsactive.png){:: style="height:35px; display:inline-block;"}
message means that a binding is currently running on your environment and is accessible.

In case of disconnection a ![web socket connection failed](images/message_wsfailed.png){:: style="height:35px; display:inline-block;"}
message is displayed. The devtool will make 5 attempts to reconnect automatically,
if all attempts fail a ![can not connect](images/message_cant_connect.png){:: style="height:35px; display:inline-block;"} message will appear.

In order to try to reconnect, press ![connect](images/button_connect.png){:: style="height:35px; display:inline-block;"} button.
You can learn more about redpesk by pressing the ![documentation](images/button_docs.png){:: style="height:35px; display:inline-block;"} button.

### Containers visualisation

On successful connection to the binding full containers tree will be requested from the binding and top 2 levels will be displayed automatically.

![Initial containers graph](./images/grapg_initial.png)

In order to display a graph for a specific container and its children:

- Click the ![get node](images/button_getnode.png){:: style="height:35px; display:inline-block;"}
- In the dialog box, from the dropdown list, select a container as a top level parent
- Select the depth for the graph:
  - **n == -1** for a full tree starting from the selected container
  - **n == 0** will display container without it's children
  - **n > 0** will display a graph for the selected container and **n** levels of children, grandchildren etc...
  - Click the ![get tree](images/button_gettree.png){:: style="height:35px; display:inline-block;"} button, to display the graph
*(initially only the 2 top levels will be displayed)*
- Click the ![collapse tree](images/button_collapse.png){:: style="height:35px; display:inline-block;"} button, to collapse the graph except for the 2 top levels.
- Click the ![full tree](images/button_fulltree.png){:: style="height:35px; display:inline-block;"} button, on top of the graph view, to display initial full tree view.

![Demo containers visualisation](./videos/binding_graphui.webm)

Hovering the mouse over a graph node will display a tooltip with its redpath *(containers installation path on the board)* and its children.
Clicking on a node will unfold its children nodes, if exist, and will open a configuration view.

### Container's configuration details

 Once the hierarchy graph is displayed:

- Click on the graph node to open a side window with it's configuration
- Click the ![display button_merged config](images/button_merged.png){:: style="height:35px; display:inline-block;"} button to display merged configuration *( containers configuration and it's parent configuration )*
- Use search bar to find any string of characters within the configuration file *( minimum 3 characters are required)
- Click the ![show conflicts](images/button_showconflicts.png){:: style="height:35px; display:inline-block;"} button to display merged configuration conflicts only
- Click the ![toggle all config fields](images/button_toggleaccordion.png){:: style="height:35px; display:inline-block;"} button to open or collapse all configuration fields
- Click the ![toggle full screen](images/button_togglefullscreen.png){:: style="height:35px; display:inline-block;"} button to toggle full screen/normal mode for configuration window

![containers hierarchy graph](./videos/config_view.webm){:: style="margin:auto; display:flex; width:75%;"}
