# 4.Character-animation
make customer npc that plays differnet animation that match the player's action


https://github.com/user-attachments/assets/c17153a7-ec24-471f-8e6e-e08b92f80f1d

first customer will spawn in outside of the place and set the goal to be inside of theplace 
<img width="667" height="203" alt="image" src="https://github.com/user-attachments/assets/7e8c9229-2693-4f43-8989-bc42a5598c03" />
<img width="650" height="313" alt="image" src="https://github.com/user-attachments/assets/99236e04-3531-49d0-8d66-6aa7cc7563d5" />
customer will have state which is walking , idle, greeting, leaving
<img width="249" height="144" alt="image" src="https://github.com/user-attachments/assets/8bef2910-75ba-4454-8223-33a3c9883d40" />
when state is changed the animator will change the animation it gonna play
<img width="454" height="439" alt="image" src="https://github.com/user-attachments/assets/31157cc7-28c8-4805-82f9-e68df449c888" />
in each render loop will check the condition with deltatime
<img width="622" height="706" alt="image" src="https://github.com/user-attachments/assets/72d5105f-28dc-4c48-a444-9692d0196823" />
<img width="476" height="198" alt="image" src="https://github.com/user-attachments/assets/98a5387f-ba75-40cb-bca5-4c4a13f445e7" />
when player interact to npc it will change to greet state for 5 second before fall back to idel state and also check too if this npc is already greeted and player have cup to give to customer, the customer will recive drink and walk off the place
<img width="433" height="246" alt="image" src="https://github.com/user-attachments/assets/cfa11e41-a350-4649-a638-9c9214eaa776" />
draw the customer 
<img width="849" height="486" alt="image" src="https://github.com/user-attachments/assets/3f64dfea-8c9e-41a3-891c-7dcf039fcf92" />



