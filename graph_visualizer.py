import sqlite3
import networkx as nx
import matplotlib.pyplot as plt
import numpy as np

# ── Connect to database ───────────────────────────────────────────────────
conn = sqlite3.connect(r"C:\Users\USER\Documents\social_media_daa\build\Desktop_Qt_6_10_2_MinGW_64_bit-Debug\socialchat.db")
cursor = conn.cursor()

# ── Build graph ───────────────────────────────────────────────────────────
G = nx.Graph()

cursor.execute("SELECT user_id, username FROM users")
for user_id, username in cursor.fetchall():
    G.add_node(user_id, username=username)

cursor.execute("SELECT user_id, friend_id FROM friends")
for user1, user2 in cursor.fetchall():
    if user1 < user2:
        G.add_edge(user1, user2)

influence = {}
cursor.execute("SELECT user_id, connection_count, influencer_percent FROM influencer_analysis")
for user_id, connections, percent in cursor.fetchall():
    influence[user_id] = (connections, percent)

# ── Layout ────────────────────────────────────────────────────────────────
pos = nx.kamada_kawai_layout(G, scale=5)

# ── Smart label offset: push label away from graph center ────────────────
def get_label_pos(pos, offset=0.28):
    """
    For each node, shift its label outward from the graph centroid.
    This prevents labels from overlapping nodes and each other.
    """
    # Find centroid of all nodes
    coords = np.array(list(pos.values()))
    cx, cy = coords[:, 0].mean(), coords[:, 1].mean()

    label_pos = {}
    for node, (x, y) in pos.items():
        # Direction vector from centroid to node
        dx, dy = x - cx, y - cy
        length = np.hypot(dx, dy)
        if length == 0:
            dx, dy = 0, 1   # default: push upward
        else:
            dx, dy = dx / length, dy / length
        # Shift label outward
        label_pos[node] = (x + dx * offset, y + dy * offset)

    return label_pos

label_pos = get_label_pos(pos, offset=0.30)

# ── Create figure ─────────────────────────────────────────────────────────
fig, ax = plt.subplots(figsize=(18, 13))
fig.patch.set_facecolor('#e8f4f8')
ax.set_facecolor('#e8f4f8')

# ── Draw edges ────────────────────────────────────────────────────────────
nx.draw_networkx_edges(G, pos, edge_color='gray', width=0.5, ax=ax, alpha=0.5)

# ── Draw nodes ────────────────────────────────────────────────────────────
nx.draw_networkx_nodes(G, pos, node_color='lightblue', node_size=500, ax=ax)

# ── Draw labels at offset positions ──────────────────────────────────────
nx.draw_networkx_labels(
    G, label_pos,
    labels={n: G.nodes[n]['username'] for n in G.nodes()},
    font_size=7,
    font_weight='bold',
    font_color='#1a1a2e',
    ax=ax
)

# ── Click interaction ─────────────────────────────────────────────────────
popup = None
highlight = None

def on_click(event):
    global popup, highlight

    if event.inaxes != ax:
        return

    if highlight:
        highlight.remove()
        highlight = None

    for node in G.nodes():
        x, y = pos[node]
        x_px, y_px = ax.transData.transform([x, y])
        dist = ((x_px - event.x) ** 2 + (y_px - event.y) ** 2) ** 0.5

        if dist < 30:
            username = G.nodes[node]['username']
            connections, percent = influence.get(node, (0, 0))

            if popup:
                popup.remove()

            globals()['popup'] = ax.annotate(
                f"@{username}\nFriends: {connections}\nInfluence: {percent:.1f}%",
                xy=pos[node],
                xytext=(15, 15),
                textcoords='offset points',
                bbox=dict(boxstyle='round,pad=0.4', facecolor='yellow',
                          alpha=0.92, edgecolor='orange', linewidth=1.5),
                fontsize=9,
                fontweight='bold',
                zorder=20
            )

            globals()['highlight'] = ax.scatter(
                x, y, s=400,
                facecolors='none',
                edgecolors='red',
                linewidths=2.5,
                zorder=15
            )

            fig.canvas.draw()
            return

    if popup:
        popup.remove()
        globals()['popup'] = None
    if highlight:
        highlight.remove()
        globals()['highlight'] = None

    fig.canvas.draw()


fig.canvas.mpl_connect('button_press_event', on_click)

# ── Final styling ─────────────────────────────────────────────────────────
plt.title("Social Network - Click any user", fontsize=13, fontweight='bold', pad=-15)
plt.axis('off')
plt.tight_layout()
plt.show()

conn.close()
