import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { User } from '../models/user';
import { UserService } from '../services/user.service';

@Component({
  selector: 'app-registrations',
  templateUrl: './registrations.component.html',
  styleUrls: ['./registrations.component.css']
})
export class RegistrationsComponent implements OnInit {

  constructor(private router: Router, private userService: UserService) { }

  users: User[] = [];
  shownUsers: User[] = [];

  ngOnInit(): void {
    let type = localStorage.getItem("type");
    if(type != 'manager') this.router.navigate(['']);
    let username = localStorage.getItem("user");
    username = username == null ? "" : username; // check if username wasnt set
    if(username == "") this.logout;
    this.userService.getNonRegistered().subscribe((data:User[])=>{
      this.users = data;
      this.shownUsers = data;
    })
  }

  logout() {
    localStorage.clear()
    this.router.navigate([''])
  }


  // Sorting properties
  private sortDirection = 1; // 1 for ascending, -1 for descending
  private currentSortKey = '';

  // Search text object
  searchText = {
    firstname: '',
    lastname: '',
    username: '',
    adress: '',
    phone: '',
    mail: ''
  };

  get filteredPatients(): User[] {
    return this.users.filter(user =>
      (user.firstname.toLowerCase().includes(this.searchText.firstname.toLowerCase()) &&
      user.lastname.toLowerCase().includes(this.searchText.lastname.toLowerCase()) &&
      user.username.toLowerCase().includes(this.searchText.username.toLowerCase()) &&
      user.adress.toLowerCase().includes(this.searchText.adress.toLowerCase()) &&
      user.phone.toLowerCase().includes(this.searchText.phone.toLowerCase()) &&
      user.mail.toLowerCase().includes(this.searchText.mail.toLowerCase()))
    );
  }

  applyFilters(): void {
    this.shownUsers = this.filteredPatients
  }

  acceptPatient(username: string){
    this.userService.acceptPatient(username).subscribe(resp=>{
      alert(resp['message'])
      this.userService.getNonRegistered().subscribe((data:User[])=>{
        this.users = data;
        this.shownUsers = data;
      })
    })
  }

  removePatient(username: string, mail: string){
    this.userService.removePatient(username,mail).subscribe(resp=>{
      alert(resp['message'])
      this.userService.getNonRegistered().subscribe((data:User[])=>{
        this.users = data;
        this.shownUsers = data;
      })
    })
  }

}
